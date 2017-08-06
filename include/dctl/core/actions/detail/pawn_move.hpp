#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/pattern.hpp> // move_targets
#include <dctl/core/board/angle.hpp>            // angle, rotate
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/state/color_piece.hpp>      // color, color_, pawn_
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_plus, foldl_comma
#include <dctl/util/type_traits.hpp>            // board_t, set_t, value_t

namespace dctl::core {
namespace detail {

template<class...>
class pawn_move;

template<color Side, class Reverse, class State>
class pawn_move<color_<Side>, Reverse, State>
{
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;

        template<class Direction>
        using rot = meta::integral_c<int, rotate(angle{Direction::value}, angle{orientation}).value()>;

        using pawn_move_directions = meta::transform<rot, basic_pawn_move_directions>;
public:
        static auto detect(State const& s) noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::foldl_logical_or<pawn_move_directions>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return !move_targets<board_type, Direction>{}(pawns, s.pieces(empty_c))
                                        .empty()
                                ;
                        });
                }
                return false;
        }

        static auto count(State const& s) noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::foldl_plus<pawn_move_directions>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return move_targets<board_type, Direction>{}(pawns, s.pieces(empty_c))
                                        .count()
                                ;
                        });
                }
                return 0;
        }

        template<class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        meta::foldl_comma<pawn_move_directions>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                move_targets<board_type, Direction>{}(pawns, s.pieces(empty_c))
                                        .consume([&](auto dest_sq) {
                                                seq.emplace_back(
                                                        prev<board_type, Direction>{}(dest_sq),
                                                        dest_sq,
                                                        board_type::promotion(Side).contains(dest_sq)
                                                );
                                        })
                                ;
                        });
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
