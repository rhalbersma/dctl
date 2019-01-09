#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // _deg, rotate
#include <dctl/core/state/color.hpp>    // white
#include <type_traits>                  // false_type

namespace dctl::core {

template<class Board, class Color, class Reverse = std::false_type>
constexpr auto bearing_v = rotate(Board::orientation, ((Color{} == white_c) ^ Reverse{}) ? 0_deg : 180_deg);

}       // namespace dctl::core
