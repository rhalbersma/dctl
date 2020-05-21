#pragma once

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // dir_...
#include <tabula/tuple.hpp>                     // tuple_c

namespace dctl::core {
namespace block_adl {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play
struct pool
{
        constexpr static auto width                     = 8;            // 1
        constexpr static auto height                    = 8;            // 1
        constexpr static auto coloring                  = 1;            // 2 (and 4)
        constexpr static auto initial_position_gap      = 2;            // 5

        constexpr static auto is_backward_pawn_jump     = true;         // 14
        constexpr static auto is_long_ranged_king       = true;         // 15
        constexpr static auto is_superior_rank_jump     = false;        // 16
        constexpr static auto is_passing_promotion      = false;        // 16 (and 22 and 23)
        constexpr static auto is_passing_capture        = false;        // 19 (and 17 and 18)
        constexpr static auto is_quantity_precedence    = false;        // 20

        constexpr static auto pawn_jump_directions = tabula::tuple_c<int, dir_NE, dir_NW, dir_SW, dir_SE>;

        constexpr static auto max_repetitions           = 3;            // 26
        constexpr static auto max_reversible_moves      = 30;           // 26
};

}       // namespace block_adl

using block_adl::pool;

}       // namespace dctl::core
