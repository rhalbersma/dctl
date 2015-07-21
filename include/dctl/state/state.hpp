#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/action/action.hpp>
#include <dctl/state/mrp_kings/mrp_kings.hpp>
#include <dctl/state/mrp_kings/zobrist.hpp>
#include <dctl/state/piece_placement.hpp>
#include <dctl/state/player_to_move.hpp>
#include <dctl/state/to_move/to_move.hpp>
#include <dctl/state/to_move/zobrist.hpp>
#include <dctl/state/reversible_actions.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/set_type.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <cassert>                      // assert
#include <tuple>
#include <type_traits>                  // false_type, true_type


namespace dctl {

template<class Rules, class Board>
class State
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using   set_type = get_set_type<Board>;

private:
        detail::wma::PiecePlacement<Board> piece_placement_{};
        PlayerToMove player_to_move_{};

public:
        // initialize with a set of bitboards and a color
        State(set_type const& black, set_type const& white, set_type const& pawns, set_type const& kings, Color c)
        :
                piece_placement_{black, white, pawns, kings},
                player_to_move_{c}
        {}

        static State initial(int separation = initial_position_gap_v<Rules> + Board::height() % 2)
        {
                auto const b = board::Initial<Board>::mask(Color::black, separation);
                auto const w = board::Initial<Board>::mask(Color::white, separation);
                return { b, w, b | w, set_type{}, Color::white };
        }

        template<class Action>
        auto& make(Action const& a)
        {
                piece_placement_.make(a);
                player_to_move_.make(a);
                return *this;
        }

        template<class Action>
        auto& undo(Action const& a)
        {
                player_to_move_.undo(a);
                piece_placement_.undo(a);
                return *this;
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

        auto to_move() const noexcept
        {
                return player_to_move_;
        }

        auto is_to_move(Color c) const noexcept
        {
                return player_to_move_ == c;
        }

        auto hash() const
        {
                return std::size_t{0};
        }
};

}       // namespace dctl
