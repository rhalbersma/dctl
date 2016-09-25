#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/mask/jump_targets.hpp>                   // jump_targets
#include <dctl/color_piece.hpp>                         // Color, color_constant, pawn_type
#include <dctl/rule_traits.hpp>                         // is_backward_pawn_jump, is_orthogonal_jump
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <type_traits>                                  // is_same

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State>
class Detect<color_constant<Side>, pawn_type, select::jump, Reverse, State>
{
        using color_type = color_constant<Side>;
        using piece_type = pawn_type;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = mask::jump_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = bearing_v<board_type, color_type, Reverse>.degrees;
public:
        auto operator()(State const& state) const noexcept
        {
                if (auto const sources = state.pieces(color_type{}, piece_type{}); sources.any()) {
                        return directions(sources, state.pawn_targets(color_type{}), state.not_occupied());
                }
                return false;
        }
private:
        auto directions(set_type const sources, set_type const pawn_targets, set_type const not_occupied) const noexcept
        {
                if constexpr (!is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right_up, left_up>(
                                sources, pawn_targets, not_occupied
                        );
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right_up, left_up, left_down, right_down>(
                                sources, pawn_targets, not_occupied
                        );
                }
                if constexpr (!is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right, right_up, up, left_up, left>(
                                sources, pawn_targets, not_occupied
                        );
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right, right_up, up, left_up, left, left_down, down, right_down>(
                                sources, pawn_targets, not_occupied
                        );
                }
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const pawn_targets, set_type const not_occupied) const noexcept
        {
                return (... || jump_targets<Directions<orientation>{}>{}(sources, pawn_targets, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace dctl
