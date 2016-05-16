#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/patterns.hpp>                      // jump_targets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/rule_traits.hpp>                         // is_backward_pawn_jump, is_orthogonal_jump, is_superior_rank_jump
#include <dctl/state/piece_placement/pieces.hpp>
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::pawn, select::jump, Reverse, State>
{
        using   board_type = board_t<State>;
        using   rules_type = rules_t<State>;
        using     set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = board::jump_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
public:
        auto operator()(State const& state) const noexcept
        {
                auto const active_pawns = pieces<ToMove, Piece::pawn>(state);
                return active_pawns.any() ? directions_dispatch(
                        active_pawns, state.pawn_targets(!ToMove), state.not_occupied(),
                        pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{}
                ) : false;
        }
private:
        auto directions_dispatch(
                set_type const active_pawns, set_type const pawn_targets, set_type const not_occupied,
                forward_pawn_jump_tag, diagonal_jump_tag) const noexcept
        {
                return directions_lfold<left_up, right_up>(
                        active_pawns, pawn_targets, not_occupied
                );
        }

        auto directions_dispatch(
                set_type const active_pawns, set_type const pawn_targets, set_type const not_occupied,
                backward_pawn_jump_tag, diagonal_jump_tag) const noexcept
        {
                return directions_lfold<left_up, right_up, left_down, right_down>(
                        active_pawns, pawn_targets, not_occupied
                );
        }

        auto directions_dispatch(
                set_type const active_pawns, set_type const pawn_targets, set_type const not_occupied,
                forward_pawn_jump_tag, orthogonal_jump_tag) const noexcept
        {
                return directions_lfold<up, left_up, right_up, left, right>(
                        active_pawns, pawn_targets, not_occupied
                );
        }

        auto directions_dispatch(
                set_type const active_pawns, set_type const pawn_targets, set_type const not_occupied,
                backward_pawn_jump_tag, orthogonal_jump_tag) const noexcept
        {
                return directions_lfold<up, left_up, right_up, left, right, left_down, right_down, down>(
                        active_pawns, pawn_targets, not_occupied
                );
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_pawns, set_type const pawn_targets, set_type const not_occupied) const noexcept
        {
                return (... || jump_targets<Directions<bearing.degrees()>{}>{}(active_pawns, pawn_targets, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
