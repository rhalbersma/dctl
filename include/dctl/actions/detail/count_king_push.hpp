#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/patterns.hpp>                      // push_targets
#include <dctl/color.hpp>                               // color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // king_range_category
#include <dctl/state/pieces.hpp>
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<color ToMove, class Reverse, class State>
class Count<ToMove, piece::king, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = board::push_targets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;

public:
        auto operator()(State const& state) const noexcept
        {
                auto const active_kings = pieces<ToMove, piece::king>(state);
                return active_kings.any() ? directions_lfold<left_up, right_up, left_down, right_down>(active_kings, state.not_occupied()) : 0;
        }

private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_kings, set_type const not_occupied) const noexcept
        {
                return (... + king_push_targets<Directions<bearing.degrees>{}>{}(active_kings, not_occupied).count());
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
