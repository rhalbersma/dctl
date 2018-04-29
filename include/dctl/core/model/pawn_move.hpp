#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle, rotate
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/model/pattern.hpp>          // move_squares
#include <dctl/core/rules/type_traits.hpp>      // basic_pawn_move_directions_v
#include <dctl/core/state/color_piece.hpp>      // color, color_
#include <boost/hana/fold.hpp>                  // fold
#include <boost/hana/for_each.hpp>              // for_each
#include <boost/hana/integral_constant.hpp>     // int_c
#include <boost/hana/or.hpp>                    // or_
#include <boost/hana/plus.hpp>                  // plus
#include <boost/hana/transform.hpp>             // transform

namespace dctl::core {
namespace detail {

template<class...>
class pawn_move;

template<class Rules, class Board, color Side, class Reverse>
class pawn_move<Rules, Board, color_<Side>, Reverse>
{
        constexpr static auto orientation = bearing_v<Board, color_<Side>, Reverse>;

        constexpr static auto pawn_move_directions = boost::hana::transform(basic_pawn_move_directions_v<Rules>, [](auto const dir) {
                return boost::hana::int_c<rotate(angle{dir}, angle{orientation}).value()>;
        });
public:
        template<class Set>
        static auto detect(Set const& pawns, Set const& empty) noexcept
        {
                return boost::hana::fold(
                        boost::hana::transform(pawn_move_directions, [&](auto const dir) {
                                return !move_squares<Board, decltype(dir)>{}(pawns, empty).empty();
                        }), 
                        boost::hana::or_
                );
        }

        template<class Set>
        static auto count(Set const& pawns, Set const& empty) noexcept
        {
                return boost::hana::fold(
                        boost::hana::transform(pawn_move_directions, [&](auto const dir) {
                                return move_squares<Board, decltype(dir)>{}(pawns, empty).count();
                        }), 
                        boost::hana::plus
                );
        }

        template<class Set, class SequenceContainer>
        static auto generate(Set const& pawns, Set const& empty, SequenceContainer& seq)
        {
                boost::hana::for_each(pawn_move_directions, [&](auto const dir) {
                        using direction_t = decltype(dir);
                        move_squares<Board, direction_t>{}(pawns, empty).for_each([&](auto const dest_sq) {
                                seq.emplace_back(prev<Board, direction_t>{}(dest_sq), dest_sq, Board::promotion(Side).contains(dest_sq));
                        });
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
