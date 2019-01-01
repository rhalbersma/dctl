#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

struct thai
{
        constexpr static auto is_long_ranged_king = true;
        constexpr static auto is_backward_pawn_jump = false;

        constexpr static auto initial_position_gap = 4;
        constexpr static auto is_land_behind_piece = true;
        constexpr static auto is_passing_capture = true;
        constexpr static auto jumpsep = '-';
};

}       // namespace block_adl

using block_adl::thai;

}       // namespace dctl::core
