#pragma once
#include <dctl/position/position_fwd.hpp>
#include <dctl/bit.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/move/move.hpp>
#include <dctl/position/piece_placement/piece_placement.hpp>
#include <dctl/position/active_color/active_color.hpp>
#include <dctl/position/reversible_moves.hpp>
#include <dctl/position/mru_king/mru_king.hpp>
#include <dctl/position/mru_king/zobrist.hpp>
#include <dctl/position/color.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <cassert>                      // assert
#include <tuple>
#include <type_traits>                  // false_type, true_type

namespace dctl {

template<class Rules, class Board>
auto init_hash(Position<Rules, Board> const& p);

template <class Rules, class Board>
struct Position
{
public:
        using rules_type = Rules;
        using board_type = Board;
        using set_type = set_type_t<Board>;
        using Set = set_type;
        using TreeIterator = Position const*;
        static constexpr auto gap = initial_gap_v<Rules> + Board::height % 2;

        // initialize with a set of bitboards and a color
        Position(Set const& black_pieces, Set const& white_pieces, Set const& kings, bool side_to_move)
        :
                piece_placement_{black_pieces, white_pieces, kings},
                active_color_{side_to_move}
        {
                hash_ = init_hash(*this);
        }

        Position(
                Set const& black_pawns, Set const& black_kings,
                Set const& white_pawns, Set const& white_kings,
                bool side_to_move
        )
        :
                piece_placement_{black_pawns, black_kings, white_pawns, white_kings},
                active_color_{side_to_move}
        {
                hash_ = init_hash(*this);
        }

        static Position initial(int separation = gap)
        {
                return {
                        board::Initial<Board>::mask(Color::black, separation),
                        board::Initial<Board>::mask(Color::white, separation),
                        Set{},
                        Color::white
                };
        }

        // queries

        auto kings(bool color) const
        {
                return piece_placement_.kings(color);
        }

        auto pawns(bool color) const
        {
                return piece_placement_.pawns(color);
        }

        auto pieces(bool color) const
        {
                return piece_placement_.pieces(color);
        }

        auto kings() const
        {
                return piece_placement_.kings();
        }

        auto pawns() const
        {
                return piece_placement_.pawns();
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

        auto active_color() const
        {
                return active_color_;
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

        auto const& mru_king(bool color) const
        {
                return mru_king_[color];
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
                active_color_.make(m, hash_);

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
                make_mru_kings(m);
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
        void make_active_mru_king(MostRecentlyUsedKing<Rules, Board>& mru, Move const& m)
        {
                if (mru.is_active()) {
                        hash_ ^= init_hash(mru, m.active_color());
                        if (m.is_reversible()) {
                                if (m.from() != mru.square())
                                        mru.init(m.dest());
                                else
                                        mru.increment(m.dest());
                        } else {
                                mru.reset();
                        }
                        hash_ ^= init_hash(mru, m.active_color());
                }

                if (m.is_promotion()) {
                        // the first of multiple pawns
                        if (!mru.is_active() && set_multiple(pawns(m.active_color())))
                                mru.activate();
                        // the single last pawn
                        if (mru.is_active() && set_single(pawns(m.active_color())))
                                mru.deactivate();
                }
        }

        template<class Move>
        void make_passive_mru_king(MostRecentlyUsedKing<Rules, Board>& mru, Move const& m)
        {
                if (!mru.is_active() || !m.is_jump())
                        return;

                // capture all kings or all pawns
                auto const deactivate =
                        set_includes(m.captured_kings(),  kings(m.active_color())) ||
                        set_includes(m.captured_pieces(), pawns(m.active_color()))
                ;

                // capture the most recently used king
                if (deactivate || m.captured_kings().test(mru.square())) {
                        hash_ ^= init_hash(mru, !m.active_color());
                        mru.reset();
                        hash_ ^= init_hash(mru, !m.active_color());
                }

                if (deactivate)
                        mru.deactivate();
        }

        template<class Move>
        void make_mru_kings(Move const& m)
        {
                make_active_mru_king(mru_king_[m.active_color()], m);
                make_passive_mru_king(mru_king_[!m.active_color()], m);
        }

        bool hash_invariant() const
        {
                return hash_ == init_hash(*this);
        }

        // representation

        PiecePlacement<Rules, Board> piece_placement_{};
        ActiveColor active_color_{};
        ReversibleMoves reversible_moves_{};
        MostRecentlyUsedKing<Rules, Board> mru_king_[2]{};
        TreeIterator parent_{};
        uint64_t hash_{};
        int distance_to_root_{};
};

template<class Position>
auto grand_parent(Position const& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<class Position>
decltype(auto) active_restricted(Position const& p)
{
        return p.restricted()[p.active_color()];
}

template<class Position>
decltype(auto) passive_restricted(Position const& p)
{
        return p.restricted()[!p.active_color()];
}

template<class Rules, class Board>
auto init_hash(Position<Rules, Board> const& p)
{
        return
                init_hash(p.piece_placement())                  ^
                init_hash(p.active_color())                     ^
                init_hash(p.mru_king(Color::black), Color::black) ^
                init_hash(p.mru_king(Color::white), Color::white)
        ;
}

}       // namespace dctl
