#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/count_primary_fwd.hpp>       // count (primary template)
#include <dctl/core/actions/select/push.hpp>                    // push
#include <dctl/core/board/angle.hpp>                            // left_up, right_up, left_down, right_down
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/ray.hpp>                              // make_iterator, rotate, mirror
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
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = push_targets<board_type, Direction, king_range_category_t<rules_type>>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>.value();

public:
        auto operator()(State const& s) const noexcept
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        auto result = 0;
                        xstd::for_each(s.pieces(to_move_c, kings_c), [&](auto const from_sq) {
                                result += ray::king_moves<rules_type, board_type>{}(from_sq, s.pieces(occup_c)).count();
                        });
                        return result;
                } else {
                        if (auto const kings = s.pieces(to_move_c, kings_c); !kings.empty()) {
                                return foldr_plus<right_up, left_up, left_down, right_down>(kings, s.pieces(empty_c));
                        }
                        return 0;
                }
        }

private:
        template<template<int> class... Directions>
        auto foldr_plus(set_type const kings, set_type const empty) const noexcept
        {
                return (king_push_targets<Directions<orientation>{}>{}(kings, empty).count() + ...);
        }
};

}       // namespace detail
}       // namespace dctl::core
