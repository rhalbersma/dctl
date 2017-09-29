#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/pattern.hpp> // move_squares
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/state/color_piece.hpp>      // color, color_
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_plus, foldl_comma

namespace dctl::core {
namespace detail {

template<class...>
class pawn_move;

template<class Rules, class Board, color Side, class Reverse>
class pawn_move<Rules, Board, color_<Side>, Reverse>
{
        constexpr static auto orientation = bearing_v<Board, color_<Side>, Reverse>;

        template<class Arg>
        using oriented = meta::integral_c<int, rotate_v<Arg::value, orientation>>;

        using pawn_move_directions = meta::transform<oriented, basic_pawn_move_directions>;
public:
        template<class Set>
        static auto detect(Set const& pawns, Set const& empty) noexcept
        {
                return meta::foldl_logical_or<pawn_move_directions>{}([&](auto const dir) {
                        constexpr auto dir_c = decltype(dir){};
                        return !move_squares<Board, dir_c>{}(pawns, empty).empty();
                });
        }

        template<class Set>
        static auto count(Set const& pawns, Set const& empty) noexcept
        {
                return meta::foldl_plus<pawn_move_directions>{}([&](auto const dir) {
                        constexpr auto dir_c = decltype(dir){};
                        return move_squares<Board, dir_c>{}(pawns, empty).count();
                });
        }

        template<class Set, class SequenceContainer>
        static auto generate(Set const& pawns, Set const& empty, SequenceContainer& seq)
        {
                meta::foldl_comma<pawn_move_directions>{}([&](auto const dir) {
                        constexpr auto dir_c = decltype(dir){};
                        move_squares<Board, dir_c>{}(pawns, empty).for_each([&](auto const dest_sq) {
                                seq.emplace_back(prev<Board, dir_c>{}(dest_sq), dest_sq, Board::promotion(Side).contains(dest_sq));
                        });
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
