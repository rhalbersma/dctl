#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/count_primary_fwd.hpp>       // count (primary template)
#include <dctl/core/actions/select/push.hpp>                    // push
#include <dctl/core/board/angle.hpp>                            // left_up, right_up
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/push_targets.hpp>                     // push_targets
#include <dctl/core/rules/type_traits.hpp>                      // short_ranged_tag
#include <dctl/core/state/color_piece.hpp>                      // color, color_, pawn_
#include <dctl/util/meta.hpp>                                   // map_reduce, plus
#include <dctl/util/type_traits.hpp>                            // board_t, set_t

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State>
class count<color_<Side>, pawns_, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
        using pawn_push_directions = std::tuple<right_up<orientation>, left_up<orientation>>;
public:
        auto operator()(State const& s) const noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::map_reduce<pawn_push_directions, meta::plus>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return push_targets<board_type, Direction, short_ranged_tag>{}(pawns, s.pieces(empty_c)).count();
                        });
                }
                return 0;
        }
};

}       // namespace detail
}       // namespace dctl::core
