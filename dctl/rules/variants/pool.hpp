#pragma once
#include <dctl/rules/variants/pool_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value.hpp>

namespace dctl {
namespace rules {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Pool
{
        // main rules
        typedef range::distance_N king_range;                                   // 15
        typedef directions::diag pawn_jump_directions;                          // 13
        typedef precedence::none jump_precedence;                               // 20

        // drawing rules
        typedef std::integral_constant<int,  3> max_repetitions;                // 26
        typedef std::integral_constant<int, 30> max_reversible_moves;           // 26
        typedef std::integral_constant<int, 13> max_3Kv1K_minority_moves;       // 27
        typedef std::integral_constant<int,  5> max_LLv1K_majority_moves;       // 28
};

}       // namespace rules
}       // namespace dctl
