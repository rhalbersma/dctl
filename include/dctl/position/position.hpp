#pragma once
#include <dctl/position/position_fwd.hpp>
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/move/move.hpp>
#include <dctl/position/mrp_kings/mrp_kings.hpp>
#include <dctl/position/mrp_kings/zobrist.hpp>
#include <dctl/position/piece_placement/piece_placement.hpp>
#include <dctl/position/piece_placement/zobrist.hpp>
#include <dctl/position/to_move/to_move.hpp>
#include <dctl/position/to_move/zobrist.hpp>
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

template<class Rules, class Board>
class Position
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using   set_type = dctl::set_type<Board>;
        using TreeIterator = Position const*;

private:
        PiecePlacement<Rules, Board> piece_placement_{};
        ReversibleMoves reversible_moves_{};
        //MostRecentlyPushedKings<Rules, Board> mrp_kings_;
        TreeIterator parent_{};
        uint64_t hash_{};
        unsigned distance_to_root_{};
        Color to_move_{};

        bool hash_invariant() const
        {
                return hash_ == hash_xor_accumulate(*this);
        }

public:
        // initialize with a set of bitboards and a color
        Position(set_type const& black, set_type const& white, set_type const& pawns, set_type const& kings, Color c)
        :
                piece_placement_{black, white, pawns, kings},
                //mrp_kings_{piece_placement_},
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

        template<class Move>
        auto make(Move const& m)
        {
                //assert(is_pseudo_legal(*this, m));

                piece_placement_.make(m);
                flip(to_move_);
                make_irreversible(m);

                hash_ ^= hash_xor_accumulate(zobrist::PiecePlacement<set_type::size()>{}, m, xstd::type_is<decltype(piece_placement_)>{});
                hash_ ^= hash_xor_accumulate(zobrist::ToMove<>{}, m, xstd::type_is<Color>{});

                assert(hash_invariant());
        }

        void attach(Position const& other)
        {
                parent_ = &other;       // link the pointers
        }

        // observers

        decltype(auto) pieces(Color c) const
        {
                return piece_placement_.pieces(c);
        }

        decltype(auto) pieces(Piece p) const
        {
                return piece_placement_.pieces(p);
        }

        decltype(auto) pieces(Color c, Piece p) const
        {
                return piece_placement_.pieces(c, p);
        }

        decltype(auto) pieces() const
        {
                return piece_placement_.pieces();
        }

        decltype(auto) not_occupied() const
        {
                return piece_placement_.not_occupied();
        }

        auto num_pieces(Color c, Piece p) const noexcept
        {
                return piece_placement_.num_pieces(c, p);
        }

        auto to_move() const
        {
                return to_move_;
        }
/*
        auto index(Color c) const
        {
                return mrp_kings_.index(c);
        }

        auto count(Color c) const
        {
                return mrp_kings_.count(c);
        }

        auto is_tracked(Color c) const
        {
                return mrp_kings_.is_tracked(c);
        }

        auto is_counted(Color c) const
        {
                return mrp_kings_.is_counted(c);
        }

        auto is_limited(Color c) const
        {
                return mrp_kings_.is_limited(c);
        }
*/
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

        auto distance_to_root() const
        {
                return distance_to_root_;
        }

        friend auto hash_xor_accumulate(Position const& p)
        {
                enum { NumSquares = set_type::size() };
                //enum { M = MostRecentlyPushedKings<Rules, Board>::M };
                //enum { N = MostRecentlyPushedKings<Rules, Board>::N };

                return
                        hash_xor_accumulate(zobrist::PiecePlacement<NumSquares>{}   , p.piece_placement_) ^
                        hash_xor_accumulate(zobrist::ToMove<>{}                     , p.to_move_        ) //^
                        //hash_xor_accumulate(zobrist::MostRecentlyPushedKings<M, N>{}, p.mrp_kings_      )
                ;
        }

private:
        // implementation

        template<class Move>
        auto make_irreversible(Move const& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible(m, is_restricted_king_push_t<Rules>{});
        }

        // restricted consecutive moves with the same king
        template<class Move>
        auto make_irreversible(Move const& m, std::true_type)
        {
                make_irreversible(m, std::false_type{});
                //make_mrp_kings(m);
        }

        // unrestricted consecutive moves with the same king
        template<class Move>
        auto make_irreversible(Move const& m, std::false_type)
        {
                reversible_moves_.make(m);
                make_distance_to_root();
        }
/*
        template<class Move>
        void make_mrp_kings(Move const& m)
        {
                enum { M = MostRecentlyPushedKings<Rules, Board>::M };
                enum { N = MostRecentlyPushedKings<Rules, Board>::N };
                static auto const z = zobrist::MostRecentlyPushedKings<M, N>{};

                hash_ ^= hash_xor_accumulate(z, mrp_kings_);
                mrp_kings_.make(m);
                hash_ ^= hash_xor_accumulate(z, mrp_kings_);
        }
*/
        auto make_distance_to_root()
        {
                ++distance_to_root_;
        }
};

template<class Position>
auto grand_parent(Position const& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

}       // namespace dctl
