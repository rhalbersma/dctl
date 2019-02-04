#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

// http://www.usacheckers.com/rulesofcheckers.php
struct checkers
{
        // THE DRAUGHTBOARD AND ITS ARRANGEMENT
        constexpr static auto width                     = 8;            // 1.1
        constexpr static auto height                    = 8;            // 1.1
        constexpr static auto coloring                  = 1;            // 1.4
        constexpr static auto is_algebraic_notation     = false;        // 1.5

        // DRAUGHT PIECES AND THEIR ARRANGEMENT
        constexpr static auto initial_position_gap      = 2;            // 1.11

        // Ordinary Move Of A King
        constexpr static auto is_long_ranged_king       = false;        // 1.17

        // Capturing Move Of A Man
        constexpr static auto is_backward_pawn_jump     = false;        // 1.18
        constexpr static auto is_superior_rank_jump     = false;        // 1.18

        // Capturing In General
        constexpr static auto is_passing_promotion      = false;        // 1.19
        constexpr static auto is_passing_capture        = false;        // 1.19
        constexpr static auto is_quantity_precedence    = false;        // 1.20

        // Definition of a Draw
        constexpr static auto max_repetitions           = 3;            // 1.32.1
        constexpr static auto max_reversible_moves      = 80;           // 1.32.2
};

}       // namespace block_adl

using block_adl::checkers;

}       // namespace dctl::core
