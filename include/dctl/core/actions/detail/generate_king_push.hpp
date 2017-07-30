#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/core/actions/select/push.hpp>                 // push
#include <dctl/core/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/core/board/bearing.hpp>                       // bearing
#include <dctl/core/board/ray.hpp>                           // make_iterator
#include <dctl/core/board/push_targets.hpp>             // push_targets
#include <dctl/core/state/color_piece.hpp>                         // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/util/type_traits.hpp>                            // board_t, rules_t, set_t, value_t
#include <iterator>                                     // prev

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State, class SequenceContainer>
class generate<color_<Side>, kings_, select::push, Reverse, State, SequenceContainer>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = push_targets<board_type, Direction, short_ranged_tag>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>;
        SequenceContainer& m_actions;
public:
        explicit generate(SequenceContainer& seq) noexcept
        :
                m_actions{seq}
        {}

        auto operator()(State const& s) const
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        s.pieces(to_move_c, kings_c).consume([this, &s](auto const from_sq) {
                                ray::king_moves<rules_type, board_type>{}(from_sq, s.pieces(occup_c)).consume([this, from_sq](auto const dest_sq) {
                                        m_actions.emplace_back(from_sq, dest_sq);
                                });
                        });
                } else {
                        if (auto const kings = s.pieces(to_move_c, kings_c); !kings.empty()) {
                                foldl_comma_serialize<right_up, left_up, left_down, right_down>(kings, s.pieces(empty_c));
                        }
                }
        }
private:
        template<template<int> class... Directions>
        auto foldl_comma_serialize(set_type const kings, set_type const empty) const
        {
                (... , serialize<Directions<orientation>{}>(kings, empty));
        }

        template<int Direction>
        auto serialize(set_type const kings, set_type const empty) const
        {
                king_push_targets<Direction>{}(kings, empty).consume([this](auto const dest_sq) {
                        m_actions.emplace_back(
                                *std::prev(ray::make_iterator<board_type, Direction>(dest_sq)),
                                dest_sq
                        );
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
