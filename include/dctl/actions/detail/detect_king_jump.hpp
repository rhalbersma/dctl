#pragma once
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/mask/jump_targets.hpp>                   // jump_targets
#include <dctl/piece.hpp>                               // king_type
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace detail {

template<class Color , class Reverse, class State>
class Detect<Color, king_type, select::jump, Reverse, State>
{
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = mask::jump_targets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto orientation = bearing_v<board_type, Color, Reverse>.degrees;
public:
        auto operator()(State const& state) const noexcept
        {
                if (auto const active_kings = state.pieces(Color{}, king_type{}); active_kings.any()) {
                        return directions(active_kings, state.king_targets(Color{}), state.not_occupied());
                }
                return false;
        }
private:
        auto directions(set_type const active_kings, set_type const king_targets, set_type const not_occupied) const noexcept
        {
                if constexpr (is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right, right_up, up, left_up, left, left_down, down, right_down>(
                                active_kings, king_targets, not_occupied
                        );
                } else {
                        return directions_lfold<right_up, left_up, left_down, right_down>(
                                active_kings, king_targets, not_occupied
                        );
                }
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_kings, set_type const king_targets, set_type const not_occupied) const noexcept
        {
                return (... || jump_targets<Directions<orientation>{}>{}(active_kings, king_targets, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace dctl
