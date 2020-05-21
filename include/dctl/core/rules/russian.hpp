#pragma once

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // dir_...
#include <tabula/tuple.hpp>             // tuple_c

namespace dctl::core {
namespace block_adl {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct russian
{
        constexpr static auto is_long_ranged_king = true;       // 1.4.5

        constexpr static auto is_backward_pawn_jump = true;     // 1.5.3
        constexpr static auto is_passing_promotion = true;      // 1.5.15

        constexpr static auto pawn_jump_directions = tabula::tuple_c<int, dir_NE, dir_NW, dir_SW, dir_SE>;

        constexpr static auto is_algebraic_notation = true;
        constexpr static auto jumpsep = ':';
};

}       // namespace block_adl

using block_adl::russian;
using shashki = russian;

}       // namespace dctl::core
