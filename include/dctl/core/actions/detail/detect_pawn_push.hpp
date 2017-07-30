#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/detect_primary_fwd.hpp>   // detect (primary template)
#include <dctl/core/actions/select/push.hpp>                 // push
#include <dctl/core/board/angle.hpp>                         // left_up, right_up
#include <dctl/core/board/bearing.hpp>                       // bearing
#include <dctl/core/board/push_targets.hpp>             // push_targets
#include <dctl/core/state/color_piece.hpp>                         // color, color_, pawn_
#include <dctl/util/type_traits.hpp>                 // board_t, set_t

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State>
class detect<color_<Side>, pawns_, select::push, Reverse, State>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        constexpr static auto piece_c = pawns_c;
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using pawn_push_targets = push_targets<board_type, Direction, short_ranged_tag>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>;
public:
        auto operator()(State const& s) const noexcept
        {
                if (auto const sources = s.pieces(to_move_c, piece_c); !sources.empty()) {
                        return foldl_or_targets<right_up, left_up>(sources, s.pieces(empty_c));
                }
                return false;
        }
private:
        template<template<int> class... Directions>
        auto foldl_or_targets(set_type const sources, set_type const destinations) const noexcept
        {
                return (... || !pawn_push_targets<Directions<orientation>{}>{}(sources, destinations).empty());
        }
};

}       // namespace detail
}       // namespace dctl::core
