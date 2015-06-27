#pragma once
#include <dctl/state/state_fwd.hpp>
#include <dctl/player.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/action/action.hpp>
#include <dctl/state/mrp_kings/mrp_kings.hpp>
#include <dctl/state/mrp_kings/zobrist.hpp>
#include <dctl/state/piece_placement/piece_placement.hpp>
#include <dctl/state/piece_placement/zobrist.hpp>
#include <dctl/state/to_move/to_move.hpp>
#include <dctl/state/to_move/zobrist.hpp>
#include <dctl/state/reversible_moves.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/set_type.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <cassert>                      // assert
#include <tuple>
#include <type_traits>                  // false_type, true_type

namespace dctl {

template<class Rules, class Board>
auto hash_xor_accumulate(State<Rules, Board> const& p);

template<class Rules, class Board>
class State
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using   set_type = dctl::set_type<Board>;
        using TreeIterator = State const*;

private:
        PiecePlacement<Rules, Board> piece_placement_{};
        ReversibleActions reversible_moves_{};
        //MostRecentlyPushedKings<Rules, Board> mrp_kings_;
        TreeIterator parent_{};
        std::size_t hash_{};
        unsigned distance_to_root_{};
        Player to_move_{};

        bool hash_invariant() const
        {
                return hash_ == hash_xor_accumulate(*this);
        }

public:
        // initialize with a set of bitboards and a color
        State(set_type const& black, set_type const& white, set_type const& pawns, set_type const& kings, Player c)
        :
                piece_placement_{black, white, pawns, kings},
                //mrp_kings_{piece_placement_},
                to_move_{c}
        {
                hash_ = hash_xor_accumulate(*this);
        }

        static State initial(int separation = initial_position_gap_v<Rules> + Board::height() % 2)
        {
                auto const b = board::Initial<Board>::mask(Player::black, separation);
                auto const w = board::Initial<Board>::mask(Player::white, separation);
                return { b, w, b | w, set_type{}, Player::white };
        }

        template<class Action>
        auto make(Action const& m)
        {
                //assert(is_pseudo_legal(*this, m));

                piece_placement_.make(m);
                to_move_ = !to_move_;
                make_irreversible(m);

                hash_ ^= hash_xor_accumulate(zobrist::PiecePlacement<set_type::size()>{}, m, xstd::type_is<decltype(piece_placement_)>{});
                hash_ ^= hash_xor_accumulate(zobrist::ToMove<>{}, m, xstd::type_is<Player>{});

                assert(hash_invariant());
        }

        void attach(State const& other)
        {
                parent_ = &other;       // link the pointers
        }

        // observers

        decltype(auto) pieces(Player c) const
        {
                return piece_placement_.pieces(c);
        }

        decltype(auto) pieces(Piece p) const
        {
                return piece_placement_.pieces(p);
        }

        decltype(auto) pieces(Player c, Piece p) const
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

        auto num_pieces(Player c, Piece p) const noexcept
        {
                return piece_placement_.num_pieces(c, p);
        }

        auto to_move() const
        {
                return to_move_;
        }
/*
        auto index(Player c) const
        {
                return mrp_kings_.index(c);
        }

        auto count(Player c) const
        {
                return mrp_kings_.count(c);
        }

        auto is_tracked(Player c) const
        {
                return mrp_kings_.is_tracked(c);
        }

        auto is_counted(Player c) const
        {
                return mrp_kings_.is_counted(c);
        }

        auto is_limited(Player c) const
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

        friend auto hash_xor_accumulate(State const& p)
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

        template<class Action>
        auto make_irreversible(Action const& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible(m, is_restricted_king_push_t<Rules>{});
        }

        // restricted consecutive moves with the same king
        template<class Action>
        auto make_irreversible(Action const& m, std::true_type)
        {
                make_irreversible(m, std::false_type{});
                //make_mrp_kings(m);
        }

        // unrestricted consecutive moves with the same king
        template<class Action>
        auto make_irreversible(Action const& m, std::false_type)
        {
                reversible_moves_.make(m);
                make_distance_to_root();
        }
/*
        template<class Action>
        void make_mrp_kings(Action const& m)
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

template<class State>
auto grand_parent(State const& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

}       // namespace dctl
