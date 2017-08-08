#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/pattern.hpp> // move_targets
#include <dctl/core/actions/detail/tables.hpp>  // king_moves
#include <dctl/core/board/angle.hpp>            // angle, rotate
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/rules/type_traits.hpp>      // is_long_ranged_king_t
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_plus, foldl_comma
#include <dctl/util/type_traits.hpp>            // board_t, rules_t

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

        template<class Direction>
        using rot = meta::integral_c<int, rotate(angle{Direction::value}, angle{orientation}).value()>;

        using king_move_directions = meta::transform<rot, basic_king_move_directions>;
public:
        static auto detect(State const& s) noexcept
        {
                auto const kings = s.pieces(color_c<Side>, kings_c);
                if (kings.empty()) {
                        return false;
                }
                return meta::foldl_logical_or<king_move_directions>{}([&, empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return !move_targets<board_type, direction_v>{}(kings, empty)
                                .empty()
                        ;
                });
        }

        static auto count(State const& s) noexcept
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        auto result = 0;
                        s.pieces(color_c<Side>, kings_c).consume([&, occup = s.pieces(occup_c)](auto from_sq) {
                                result += king_moves<rules_type, board_type>{}(from_sq, occup)
                                        .count()
                                ;
                        });
                        return result;
                } else {
                        auto const kings = s.pieces(color_c<Side>, kings_c);
                        if (kings.empty()) {
                                return 0;
                        }
                        return meta::foldl_plus<king_move_directions>{}([&, empty = s.pieces(empty_c)](auto direction) {
                                constexpr auto direction_v = decltype(direction){};
                                return move_targets<board_type, direction_v>{}(kings, empty)
                                        .count()
                                ;
                        });
                }
        }

        template<class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        s.pieces(color_c<Side>, kings_c).consume([&, occup = s.pieces(occup_c)](auto from_sq) {
                                king_moves<rules_type, board_type>{}(from_sq, occup)
                                        .consume([&](auto dest_sq) {
                                                seq.emplace_back(from_sq, dest_sq);
                                        })
                                ;
                        });
                } else {
                        auto const kings = s.pieces(color_c<Side>, kings_c);
                        if (kings.empty()) {
                                return;
                        }
                        meta::foldl_comma<king_move_directions>{}([&, empty = s.pieces(empty_c)](auto direction) {
                                constexpr auto direction_v = decltype(direction){};
                                move_targets<board_type, direction_v>{}(kings, empty)
                                        .consume([&](auto dest_sq) {
                                                seq.emplace_back(
                                                        prev<board_type, direction_v>{}(dest_sq),
                                                        dest_sq
                                                );
                                        })
                                ;
                        });
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
