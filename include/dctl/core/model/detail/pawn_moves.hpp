#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle, rotate
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/model/detail/pattern.hpp>   // move_squares
#include <dctl/core/rules/type_traits.hpp>      // pawn_move_directions_v
#include <dctl/core/state/color.hpp>            // color, color_
#include <boost/hana/fold.hpp>                  // fold
#include <boost/hana/for_each.hpp>              // for_each
#include <boost/hana/integral_constant.hpp>     // int_c
#include <boost/hana/transform.hpp>             // transform
#include <functional>                           // logical_or, plus

namespace dctl::core::model {
namespace detail {

template<class...>
class pawn_moves;

template<class Rules, class Board, color Side, class ReverseGenerator>
class pawn_moves<Rules, Board, color_<Side>, ReverseGenerator>
{
        using set_type = set_t<Board>;

        constexpr static auto pawn_move_directions = boost::hana::transform(pawn_move_directions_v<Rules>, [](auto dir) {
                return boost::hana::int_c<rotate(angle{dir}, bearing_v<Board, color_<Side>, ReverseGenerator>).value()>;
        });
public:
        static auto detect(set_type const& pawns, set_type const& empty) noexcept
        {
                return boost::hana::fold(
                        boost::hana::transform(pawn_move_directions, [&](auto dir) {
                                return !move_squares<Board, decltype(dir)>{}(pawns, empty).empty();
                        }),
                        std::logical_or{}
                );
        }

        static auto count(set_type const& pawns, set_type const& empty) noexcept
        {
                return boost::hana::fold(
                        boost::hana::transform(pawn_move_directions, [&](auto dir) {
                                return move_squares<Board, decltype(dir)>{}(pawns, empty).ssize();
                        }),
                        std::plus{}
                );
        }

        template<class SequenceContainer>
        static auto generate(set_type const& pawns, set_type const& empty, SequenceContainer& seq)
        {
                boost::hana::for_each(pawn_move_directions, [&](auto dir) {
                        using direction_t = decltype(dir);
                        for(auto const dest_sq : move_squares<Board, direction_t>{}(pawns, empty)) {
                                seq.emplace_back(prev<Board, direction_t>{}(dest_sq), dest_sq, Board::promotion(Side).contains(dest_sq));
                        }
                });
        }
};

}       // namespace detail
}       // namespace dctl::core::model
