#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // left_up, right_up, left_down, right_down
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/board/ray.hpp>              // make_iterator
#include <dctl/core/board/push_targets.hpp>     // push_targets
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_long_ranged_king_t
#include <dctl/util/meta.hpp>                   // map_reduce, comma, plus, logical_or
#include <dctl/util/type_traits.hpp>            // board_t, rules_t, set_t, value_t
#include <iterator>                             // prev

namespace dctl::core {
namespace detail {

template<class...>
class king_move;

template<color Side, class Reverse, class State>
class king_move<color_<Side>, Reverse, State>
{
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
public:
        static auto detect(State const& s) noexcept
        {
                if (auto const kings = s.pieces(color_c<Side>, kings_c); !kings.empty()) {
                        return meta::map_reduce<king_move_directions<orientation>, meta::logical_or>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return !push_targets<board_type, Direction, short_ranged_tag>{}(kings, s.pieces(empty_c))
                                        .empty()
                                ;
                        });
                }
                return false;
        }

        static auto count(State const& s) noexcept
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        auto result = 0;
                        s.pieces(color_c<Side>, kings_c).consume([&](auto from_sq) {
                                result += ray::king_moves<rules_type, board_type>{}(from_sq, s.pieces(occup_c))
                                        .count()
                                ;
                        });
                        return result;
                } else {
                        if (auto const kings = s.pieces(color_c<Side>, kings_c); !kings.empty()) {
                                return meta::map_reduce<king_move_directions<orientation>, meta::plus>{}([&](auto direction) {
                                        constexpr auto Direction = decltype(direction){};
                                        return push_targets<board_type, Direction, short_ranged_tag>{}(kings, s.pieces(empty_c))
                                                .count()
                                        ;
                                });
                        }
                        return 0;
                }
        }

        template<class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        s.pieces(color_c<Side>, kings_c).consume([&](auto from_sq) {
                                ray::king_moves<rules_type, board_type>{}(from_sq, s.pieces(occup_c))
                                        .consume([&](auto dest_sq) {
                                                seq.emplace_back(from_sq, dest_sq);
                                        })
                                ;
                        });
                } else {
                        if (auto const kings = s.pieces(color_c<Side>, kings_c); !kings.empty()) {
                                meta::map_reduce<king_move_directions<orientation>, meta::comma>{}([&](auto direction) {
                                        constexpr auto Direction = decltype(direction){};
                                        push_targets<board_type, Direction, short_ranged_tag>{}(kings, s.pieces(empty_c))
                                                .consume([&](auto dest_sq) {
                                                        seq.emplace_back(
                                                                *std::prev(ray::make_iterator<board_type, Direction>(dest_sq)),
                                                                dest_sq
                                                        );
                                                })
                                        ;
                                });
                        }
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
