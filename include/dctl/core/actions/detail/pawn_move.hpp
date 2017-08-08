#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/pattern.hpp> // move_targets
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/state/color_piece.hpp>      // color, color_, pawn_
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_plus, foldl_comma
#include <dctl/util/type_traits.hpp>            // board_t

namespace dctl::core {
namespace detail {

template<class...>
class pawn_move;

template<color Side, class Reverse, class State>
class pawn_move<color_<Side>, Reverse, State>
{
        using board_type = board_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;

        template<class Arg>
        using oriented = meta::integral_c<int, rotate_v<Arg::value, orientation>>;

        using pawn_move_directions = meta::transform<oriented, basic_pawn_move_directions>;
public:
        static auto detect(State const& s) noexcept
        {
                auto const pawns = s.pieces(color_c<Side>, pawns_c);
                if (pawns.empty()) {
                        return false;
                }
                return meta::foldl_logical_or<pawn_move_directions>{}([&pawns, empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return !move_targets<board_type, direction_v>{}(pawns, empty).empty();
                });
        }

        static auto count(State const& s) noexcept
        {
                auto const pawns = s.pieces(color_c<Side>, pawns_c);
                if (pawns.empty()) {
                        return 0;
                }
                return meta::foldl_plus<pawn_move_directions>{}([&pawns, empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return move_targets<board_type, direction_v>{}(pawns, empty).count();
                });
        }

        template<class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                auto const pawns = s.pieces(color_c<Side>, pawns_c);
                if (pawns.empty()) {
                        return;
                }
                meta::foldl_comma<pawn_move_directions>{}([&seq, &pawns, empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        move_targets<board_type, direction_v>{}(pawns, empty).consume([&](auto dest_sq) {
                                seq.emplace_back(prev<board_type, direction_v>{}(dest_sq), dest_sq, board_type::promotion(Side).contains(dest_sq));
                        });
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
