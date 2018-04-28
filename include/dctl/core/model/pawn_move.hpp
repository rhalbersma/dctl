#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/state/color_piece.hpp>      // color, color_
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_plus, foldl_comma
#include <dctl/core/model/pattern.hpp>          // move_squares
#include <boost/mp11/algorithm.hpp>             // mp_transform
#include <boost/mp11/integral.hpp>              // mp_int

namespace dctl::core {
namespace detail {

template<class...>
class pawn_move;

template<class Rules, class Board, color Side, class Reverse>
class pawn_move<Rules, Board, color_<Side>, Reverse>
{
        constexpr static auto orientation = bearing_v<Board, color_<Side>, Reverse>;

        template<class Arg>
        using oriented = boost::mp11::mp_int<rotate_v<Arg::value, orientation>>;

        using pawn_move_directions = boost::mp11::mp_transform<oriented, basic_pawn_move_directions>;
public:
        template<class Set>
        static auto detect(Set const& pawns, Set const& empty) noexcept
        {
                return meta::foldl_logical_or<pawn_move_directions>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        return !move_squares<Board, direction_t>{}(pawns, empty).empty();
                });
        }

        template<class Set>
        static auto count(Set const& pawns, Set const& empty) noexcept
        {
                return meta::foldl_plus<pawn_move_directions>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        return move_squares<Board, direction_t>{}(pawns, empty).count();
                });
        }

        template<class Set, class SequenceContainer>
        static auto generate(Set const& pawns, Set const& empty, SequenceContainer& seq)
        {
                meta::foldl_comma<pawn_move_directions>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        move_squares<Board, direction_t>{}(pawns, empty).for_each([&](auto const dest_sq) {
                                seq.emplace_back(prev<Board, direction_t>{}(dest_sq), dest_sq, Board::promotion(Side).contains(dest_sq));
                        });
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
