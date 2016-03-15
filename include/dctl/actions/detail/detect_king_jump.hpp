#pragma once
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/wave/patterns.hpp>                 // JumpTargets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::king, select::jump, Reverse, State>
{
        using   board_type = board_t<State>;
        using   rules_type = rules_t<State>;
        using     set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = JumpTargets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
public:
        auto operator()(State const& state) const noexcept
        {
                auto const active_kings = pieces<ToMove, Piece::king>(state);
                return active_kings.any() ? directions_dispatch(
                        active_kings, state.king_targets(!ToMove), state.not_occupied(),
                        jump_category_t<rules_type>{}
                ) : false;
        }
private:
        auto directions_dispatch(
                set_type const active_kings, set_type const king_targets, set_type const not_occupied,
                diagonal_jump_tag) const noexcept
        {
                return directions_lfold<left_up, right_up, left_down, right_down>(
                        active_kings, king_targets, not_occupied
                );
        }

        auto directions_dispatch(
                set_type const active_kings, set_type const king_targets, set_type const not_occupied,
                orthogonal_jump_tag) const noexcept
        {
                return directions_lfold<up, left_up, right_up, left, right, left_down, right_down, down>(
                        active_kings, king_targets, not_occupied
                );
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_kings, set_type const king_targets, set_type const not_occupied) const noexcept
        {
                return (... || jump_targets<Directions<bearing.degrees()>{}>{}(active_kings, king_targets, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
