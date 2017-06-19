#pragma once
#include <dctl/core/actions/detail/count_primary_fwd.hpp>       // count (primary template)
#include <dctl/core/actions/select/push.hpp>                    // push
#include <dctl/core/board/angle.hpp>                            // left_up, right_up, left_down, right_down
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/push_targets.hpp>                     // push_targets
#include <dctl/core/rules/type_traits.hpp>                      // king_range_category
#include <dctl/core/state/color_piece.hpp>                      // color, color_, king_
#include <dctl/util/type_traits.hpp>                            // board_t, rules_t, set_t

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State>
class count<color_<Side>, kings_, select::push, Reverse, State>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        constexpr static auto piece_c = kings_c;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = push_targets<board_type, Direction, king_range_category_t<rules_type>>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>.value();

public:
        auto operator()(State const& s) const noexcept
        {
                if (auto const sources = s.pieces(to_move_c, piece_c); !sources.empty()) {
                        return directions_lfold<right_up, left_up, left_down, right_down>(sources, s.pieces(empty_c));
                }
                return 0;
        }

private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const destinations) const noexcept
        {
                return (... + king_push_targets<Directions<orientation>{}>{}(sources, destinations).size());
        }
};

}       // namespace detail
}       // namespace dctl::core
