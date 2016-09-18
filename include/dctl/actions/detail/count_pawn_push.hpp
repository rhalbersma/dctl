#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/mask/push_targets.hpp>                   // push_targets
#include <dctl/piece.hpp>                               // pawn_type
#include <dctl/rule_traits.hpp>                         // short_ranged_tag
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <xstd/cstddef.hpp>

namespace dctl {
namespace detail {

template<class Color, class Reverse, class State>
class Count<Color, pawn_type, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using pawn_push_targets = mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = bearing_v<board_type, Color, Reverse>.degrees;

public:
        auto operator()(State const& state) const noexcept
        {
                using namespace xstd::support_literals;
                if (auto const active_pawns = state.pieces(Color{}, pawn_type{}); active_pawns.any()) {
                        return directions_lfold<right_up, left_up>(active_pawns, state.not_occupied());
                }
                return 0_zu;
        }

private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_pawns, set_type const not_occupied) const noexcept
        {
                return (... + pawn_push_targets<Directions<orientation>{}>{}(active_pawns, not_occupied).count());
        }
};

}       // namespace detail
}       // namespace dctl
