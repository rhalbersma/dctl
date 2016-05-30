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
#include <dctl/utility/type_traits.hpp>         // set_t
#include <dctl/utility/zobrist/accumulate.hpp>
#include <cassert>                              // assert
#include <tuple>

namespace dctl {

template<class Rules, class Board>
class State
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using   set_type = set_t<Board>;

private:
        bwk::PiecePlacement<Board> piece_placement_{};
        PlayerToMove player_to_move_{};

public:
        // initialize with a set of bitboards and a color
        State(set_type const& black, set_type const& white, set_type const& pawns, set_type const& kings, Color c)
        :
                piece_placement_{black, white, pawns, kings},
                player_to_move_{c}
        {}

        static State initial(int separation = initial_position_gap_or_v<Rules> - Board::height % 2)
        {
                auto const b = board::Initial<Board>::mask(Color::black, separation);
                auto const w = board::Initial<Board>::mask(Color::white, separation);
                return { b, w, b | w, set_type{}, Color::white };
        }

        template<class Action>
        auto& make(Action const& a)
        {
                piece_placement_.make(player_to_move_, a);
                player_to_move_.make(a);
                return *this;
        }

        // observers

        auto pieces(Color const c) const
        {
                return piece_placement_.pieces(c);
        }

        auto pieces(Piece const p) const
        {
                return piece_placement_.pieces(p);
        }

        auto pieces(Color const c, Piece const p) const
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

        auto king_targets(Color const c) const
        {
                return pieces(c);
        }

        auto pawn_targets(Color const c) const
        {
                return pawn_targets_dispatch(c, rank_jump_category_t<rules_type>{});
        }

        auto num_pieces(Color const c, Piece const p) const noexcept
        {
                return piece_placement_.num_pieces(c, p);
        }

        auto to_move() const noexcept
        {
                return player_to_move_;
        }

        auto is_to_move(Color const c) const noexcept
        {
                return to_move() == c;
        }

        auto hash() const
        {
                return std::size_t{0};
        }
private:
        auto pawn_targets_dispatch(Color const c, inferior_rank_jump_tag) const
        {
                return pieces(c);
        }

        auto pawn_targets_dispatch(Color const c, superior_rank_jump_tag) const
        {
                return pieces(c, Piece::pawn);
        }
};

}       // namespace dctl
