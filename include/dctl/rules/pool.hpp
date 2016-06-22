#pragma once
#include <dctl/board.hpp>       // checkers

namespace dctl {
namespace rules {
namespace block_adl {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play
struct pool
{
        using board_type = board::checkers;
        static constexpr auto initial_gap = 2;                  // 5

        static constexpr auto is_backward_pawn_jump = true;     // 14
        static constexpr auto is_long_ranged_king = true;       // 15
        static constexpr auto is_superior_rank_jump = false;    // 16
        static constexpr auto is_passing_promotion = false;     // 16 (and 22 and 23)
        static constexpr auto is_passing_capture = false;       // 19 (and 17 and 18)
        static constexpr auto is_quantity_precedence = false;   // 20

        static constexpr auto max_repetitions = 3;              // 26
        static constexpr auto max_reversible_moves = 30;        // 26
};

}       // namespace block_adl

using block_adl::pool;

}       // namespace rules
}       // namespace dctl
