#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/mask/push_targets.hpp>                   // push_targets
#include <dctl/piece.hpp>                               // king_type
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t

namespace dctl {
namespace detail {

template<class Color, class Reverse, class State>
class Detect<Color, king_type, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = bearing_v<board_type, Color, Reverse>.degrees;
public:
        auto operator()(State const& state) const noexcept
        {
                if (auto const active_kings = state.pieces(Color{}, king_type{}); active_kings.any()) {
                        return directions_lfold<right_up, left_up, left_down, right_down>(active_kings, state.not_occupied());
                }
                return false;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_kings, set_type const not_occupied) const noexcept
        {
                return (... || king_push_targets<Directions<orientation>{}>{}(active_kings, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace dctl
