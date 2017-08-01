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
#include <dctl/util/meta.hpp>                                   // map_reduce, plus
#include <dctl/util/type_traits.hpp>                            // board_t, rules_t, set_t

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State>
class count<color_<Side>, kings_, select::push, Reverse, State>
{
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
        using king_push_directions = std::tuple<right_up<orientation>, left_up<orientation>, left_down<orientation>, right_down<orientation>>;
public:
        auto operator()(State const& s) const noexcept
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        auto result = 0;
                        s.pieces(color_c<Side>, kings_c).consume([&](auto const from_sq) {
                                result += ray::king_moves<rules_type, board_type>{}(from_sq, s.pieces(occup_c)).count();
                        });
                        return result;
                } else {
                        if (auto const kings = s.pieces(color_c<Side>, kings_c); !kings.empty()) {
                                return meta::map_reduce<king_push_directions, meta::plus>{}([&](auto direction) {
                                        constexpr auto Direction = decltype(direction){};
                                        return push_targets<board_type, Direction, short_ranged_tag>{}(kings, s.pieces(empty_c)).count();
                                });
                        }
                        return 0;
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
