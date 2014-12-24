#pragma once
#include <dctl/position/position_fwd.hpp>
#include <dctl/color.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/move/move.hpp>
#include <dctl/position/active_color/active_color.hpp>
#include <dctl/position/active_color/zobrist.hpp>
#include <dctl/position/mrp_king/mrp_king.hpp>
#include <dctl/position/mrp_king/zobrist.hpp>
#include <dctl/position/piece_placement/piece_placement.hpp>
#include <dctl/position/piece_placement/zobrist.hpp>
#include <dctl/position/reversible_moves.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/set_type.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <cassert>                      // assert
#include <tuple>
#include <type_traits>                  // false_type, true_type

namespace dctl {

template<class Rules, class Board>
auto hash_xor_accumulate(Position<Rules, Board> const& p);

template <class Rules, class Board>
class Position
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using   set_type = dctl::set_type<Board>;
        using TreeIterator = Position const*;

private:
        PiecePlacement<Rules, Board> piece_placement_{};
        Color to_move_{};
        ReversibleMoves reversible_moves_{};
        MostRecentlyPushedKing<Rules, Board> mrp_king_[2]{};
        TreeIterator parent_{};
        uint64_t hash_{};
        int distance_to_root_{};

        enum { M = MostRecentlyPushedKing<Rules, Board>::M };
        enum { N = MostRecentlyPushedKing<Rules, Board>::N };

public:
        // initialize with a set of bitboards and a color
        Position(set_type const& black, set_type const& white, set_type const& pawns, set_type const& kings, Color c)
        :
                piece_placement_{black, white, pawns, kings},
                to_move_{c}
        {
                hash_ = hash_xor_accumulate(*this);
        }

        static Position initial(int separation = initial_gap_v<Rules> + Board::height() % 2)
        {
                auto const b = board::Initial<Board>::mask(Color::black, separation);
                auto const w = board::Initial<Board>::mask(Color::white, separation);
                return { b, w, b | w, set_type{}, Color::white };
        }

        // observers

        auto pieces(Color c) const
        {
                return piece_placement_.pieces(c);
        }

        auto pieces(Piece p) const
        {
                return piece_placement_.pieces(p);
        }

        auto pieces(Color c, Piece p) const
        {
                return piece_placement_.pieces(c, p);
        }

        auto pieces() const
        {
                return piece_placement_.pieces();
        }

        auto not_occupied() const
        {
                return piece_placement_.not_occupied();
        }

        auto const& piece_placement() const
        {
                return piece_placement_;
        }

        auto to_move() const
        {
                return to_move_;
        }

        auto reversible_moves() const
        {
                return reversible_moves_;
        }

        auto parent() const
        {
                return parent_;
        }

        auto hash() const
        {
                return hash_;
        }

        auto const& mrp_king(Color c) const
        {
                return mrp_king_[static_cast<bool>(c)];
        }

        auto distance_to_root() const
        {
                return distance_to_root_;
        }

        template<class Move>
        void make(Move const& m)
        {
                //assert(is_pseudo_legal(*this, m));

                make_irreversible(m);

                piece_placement_.make(m, hash_);

                hash_ ^= zobrist::ActiveColor<>::color[true];
                flip(to_move_);

                assert(hash_invariant());
        }
        
        void attach(Position const& other)
        {
                parent_ = &other;       // link the pointers
        }

private:
        // implementation

        template<class Move>
        void make_irreversible(Move const& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible(m, is_restricted_same_king_push_t<Rules>{});
        }

        // restricted consecutive moves with the same king
        template<class Move>
        void make_irreversible(Move const& m, std::true_type)
        {
                make_irreversible(m, std::false_type());
                make_mrp_kings(m);
        }

        // unrestricted consecutive moves with the same king
        template<class Move>
        void make_irreversible(Move const& m, std::false_type)
        {
                reversible_moves_.make(m);
                make_distance_to_root();
        }

        void make_distance_to_root()
        {
                ++distance_to_root_;
        }

        template<class Move>
        void make_active_mrp_king(MostRecentlyPushedKing<Rules, Board>& mru, Move const& m)
        {
                if (mru.is_active()) {
                        hash_ ^= hash_xor_accumulate(zobrist::MostRecentlyPushedKing<M, N>{}, mru, m.to_move());
                        if (m.is_reversible()) {
                                if (m.from() != mru.square())
                                        mru.init(m.dest());
                                else
                                        mru.increment(m.dest());
                        } else {
                                mru.reset();
                        }
                        hash_ ^= hash_xor_accumulate(zobrist::MostRecentlyPushedKing<M, N>{}, mru, m.to_move());
                }

                if (m.is_promotion()) {
                        // the first of multiple pawns
                        if (!mru.is_active() && set_multiple(pieces(m.to_move(), Piece::pawn)))
                                mru.activate();
                        // the single last pawn
                        if (mru.is_active() && set_single(pieces(m.to_move(), Piece::pawn)))
                                mru.deactivate();
                }
        }

        template<class Move>
        void make_passive_mrp_king(MostRecentlyPushedKing<Rules, Board>& mru, Move const& m)
        {
                if (!mru.is_active() || !m.is_jump())
                        return;

                // capture all kings or all pawns
                auto const deactivate =
                        pieces(m.to_move(), Piece::king).is_subset_of(m.captured(Piece::king)) ||
                        pieces(m.to_move(), Piece::pawn).is_subset_of(m.captured(Piece::pawn))
                ;

                // capture the most recently used king
                if (deactivate || m.captured(Piece::king).test(mru.square())) {
                        hash_ ^= hash_xor_accumulate(zobrist::MostRecentlyPushedKing<M, N>{}, mru, !m.to_move());
                        mru.reset();
                        hash_ ^= hash_xor_accumulate(zobrist::MostRecentlyPushedKing<M, N>{}, mru, !m.to_move());
                }

                if (deactivate)
                        mru.deactivate();
        }

        template<class Move>
        void make_mrp_kings(Move const& m)
        {
                make_active_mrp_king(mrp_king_[static_cast<bool>(m.to_move())], m);
                make_passive_mrp_king(mrp_king_[static_cast<bool>(!m.to_move())], m);
        }

        bool hash_invariant() const
        {
                return hash_ == hash_xor_accumulate(*this);
        }
};

template<class Position>
auto grand_parent(Position const& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<class Position>
decltype(auto) active_restricted(Position const& p)
{
        return p.restricted()[static_cast<bool>(p.to_move())];
}

template<class Position>
decltype(auto) passive_restricted(Position const& p)
{
        return p.restricted()[static_cast<bool>(!p.to_move())];
}

template<class Rules, class Board>
auto hash_xor_accumulate(Position<Rules, Board> const& p)
{
        enum { NumSquares = set_type<Board>::size() };
        enum { M = MostRecentlyPushedKing<Rules, Board>::M };
        enum { N = MostRecentlyPushedKing<Rules, Board>::N };

        return
                hash_xor_accumulate(zobrist::PiecePlacement<NumSquares>{}, p.piece_placement())                      ^
                hash_xor_accumulate(zobrist::ActiveColor<>{}             , p.to_move())                              ^
                hash_xor_accumulate(zobrist::MostRecentlyPushedKing<M, N>{}, p.mrp_king(Color::black), Color::black) ^
                hash_xor_accumulate(zobrist::MostRecentlyPushedKing<M, N>{}, p.mrp_king(Color::white), Color::white)
        ;
}

}       // namespace dctl
