#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/detect_primary_fwd.hpp>      // detect (primary template)
#include <dctl/core/actions/select/jump.hpp>    // jump
#include <dctl/core/board/angle.hpp>            // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/board/jump_targets.hpp>     // jump_targets
#include <dctl/core/state/color_piece.hpp>      // color, color_, pawn_
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, is_orthogonal_jump
#include <dctl/util/meta.hpp>                   // map_reduce, comma, plus, logical_or
#include <dctl/util/type_traits.hpp>            // board_t, rules_t, set_t
#include <type_traits>                          // conditional_t

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State>
class detect<color_<Side>, pawns_, select::jump, Reverse, State>
{
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
        using pawn_jump_directions = std::conditional_t<
                is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>,
                std::tuple<right<orientation>, right_up<orientation>, up<orientation>, left_up<orientation>, left<orientation>, left_down<orientation>, down<orientation>, right_down<orientation>>,
                std::conditional_t<
                        is_orthogonal_jump_v<rules_type>,
                        std::tuple<right<orientation>, right_up<orientation>, up<orientation>, left_up<orientation>, left<orientation>>,
                        std::conditional_t<
                                is_backward_pawn_jump_v<rules_type>,
                                std::tuple<right_up<orientation>, left_up<orientation>, left_down<orientation>, right_down<orientation>>,
                                std::tuple<right_up<orientation>, left_up<orientation>>
                        >
                >
        >;
public:
        auto operator()(State const& s) const noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::map_reduce<pawn_jump_directions, meta::logical_or>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return !jump_targets<board_type, Direction, short_ranged_tag>{}(pawns, s.targets(color_c<Side>, pawns_c), s.pieces(empty_c)).empty();
                        });
                }
                return false;
        }
};

}       // namespace detail
}       // namespace dctl::core
