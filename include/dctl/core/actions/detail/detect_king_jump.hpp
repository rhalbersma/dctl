#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/detect_primary_fwd.hpp>      // detect (primary template)
#include <dctl/core/actions/select/jump.hpp>                    // jump
#include <dctl/core/board/angle.hpp>                            // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/jump_targets.hpp>                // jump_targets
#include <dctl/core/state/color_piece.hpp>                      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>                           // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/util/type_traits.hpp>                            // board_t, rules_t, set_t

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State>
class detect<color_<Side>, kings_, select::jump, Reverse, State>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        constexpr static auto piece_c = kings_c;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_jump_targets = jump_targets<board_type, Direction, king_range_category_t<rules_type>>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>;
public:
        auto operator()(State const& s) const noexcept
        {
                if (auto const sources = s.pieces(to_move_c, piece_c); !sources.empty()) {
                        return directions(sources, s.targets(to_move_c, piece_c), s.pieces(empty_c));
                }
                return false;
        }
private:
        auto directions(set_type const sources, set_type const targets, set_type const destinations) const noexcept
        {
                if constexpr (is_orthogonal_jump_v<rules_type>) {
                        return foldl_or_targets<right, right_up, up, left_up, left, left_down, down, right_down>(
                                sources, targets, destinations
                        );
                } else {
                        return foldl_or_targets<right_up, left_up, left_down, right_down>(
                                sources, targets, destinations
                        );
                }
        }

        template<template<int> class... Directions>
        auto foldl_or_targets(set_type const sources, set_type const targets, set_type const destinations) const noexcept
        {
                return (... || !king_jump_targets<Directions<orientation>{}>{}(sources, targets, destinations).empty());
        }
};

}       // namespace detail
}       // namespace dctl::core
