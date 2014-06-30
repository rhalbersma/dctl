#pragma once
#include <dctl/rules/variants/pool_fwd.hpp>     // Pool
#include <dctl/successor/value.hpp>             // Value (primary template)

namespace dctl {
namespace rules {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Pool
{
        // main rules
        static constexpr auto is_long_ranged_king = true;                       // 15
        static constexpr auto is_backward_pawn_jump = true;                     // 13
        static constexpr auto is_jump_precedence = false;                       // 20

        // drawing rules
        static constexpr auto max_repetitions = 3;                              // 26
        static constexpr auto max_reversible_moves = 30;                        // 26
        static constexpr auto max_3Kv1K_minority_moves = 13;                    // 27
        static constexpr auto max_LLv1K_majority_moves = 5;                     // 28
};

}       // namespace rules
}       // namespace dctl
