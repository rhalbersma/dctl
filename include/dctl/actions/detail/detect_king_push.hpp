#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/mask/push_targets.hpp>                   // push_targets
#include <dctl/color_piece.hpp>                         // Color, color_constant, king_type
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State>
class Detect<color_constant<Side>, king_type, select::push, Reverse, State>
{
        using color_type = color_constant<Side>;
        using piece_type = king_type;
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = bearing_v<board_type, color_type, Reverse>.degrees;
public:
        auto operator()(State const& state) const noexcept
        {
                if (auto const sources = state.pieces(color_type{}, piece_type{}); sources.any()) {
                        return directions_lfold<right_up, left_up, left_down, right_down>(sources, state.pieces(none_type{}));
                }
                return false;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const not_occupied) const noexcept
        {
                return (... || king_push_targets<Directions<orientation>{}>{}(sources, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace dctl
