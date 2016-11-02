#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/color_piece.hpp>                         // Color, color_constant, pawn_type
#include <dctl/rule_traits.hpp>                         // short_ranged_tag
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <xstd/cstddef.hpp>

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State>
class Count<color_constant<Side>, pawn_type, select::push, Reverse, State>
{
        using color_type = color_constant<Side>;
        using piece_type = pawn_type;
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using pawn_push_targets = board::mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, color_type, Reverse>.degrees();

public:
        auto operator()(State const& state) const noexcept
        {
                using namespace xstd::support_literals;
                if (auto const sources = state.pieces(color_type{}, piece_type{}); sources.any()) {
                        return directions_lfold<board::right_up, board::left_up>(sources, state.pieces(none_type{}));
                }
                return 0_zu;
        }

private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const not_occupied) const noexcept
        {
                return (... + pawn_push_targets<Directions<orientation>{}>{}(sources, not_occupied).count());
        }
};

}       // namespace detail
}       // namespace dctl
