#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // left_up, right_up
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/board/push_targets.hpp>     // push_targets
#include <dctl/core/board/ray.hpp>              // make_iterator
#include <dctl/core/state/color_piece.hpp>      // color, color_, pawn_
#include <dctl/util/meta.hpp>                   // map_reduce, comma, plus, logical_or
#include <dctl/util/type_traits.hpp>            // board_t, set_t, value_t
#include <iterator>                             // prev

namespace dctl::core {
namespace detail {

template<class...>
class pawn_move;

template<color Side, class Reverse, class State>
class pawn_move<color_<Side>, Reverse, State>
{
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
public:
        static auto detect(State const& s) noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::map_reduce<pawn_move_directions<orientation>, meta::logical_or>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return !push_targets<board_type, Direction, short_ranged_tag>{}(pawns, s.pieces(empty_c)).empty();
                        });
                }
                return false;
        }

        static auto count(State const& s) noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::map_reduce<pawn_move_directions<orientation>, meta::plus>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return push_targets<board_type, Direction, short_ranged_tag>{}(pawns, s.pieces(empty_c)).count();
                        });
                }
                return 0;
        }

        template<class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        meta::map_reduce<pawn_move_directions<orientation>, meta::comma>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                push_targets<board_type, Direction, short_ranged_tag>{}(pawns, s.pieces(empty_c)).consume([&](auto dest_sq) {
                                        seq.emplace_back(
                                                *std::prev(ray::make_iterator<board_type, Direction>(dest_sq)),
                                                dest_sq,
                                                board_type::promotion(Side).contains(dest_sq)
                                        );
                                });
                        });
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
