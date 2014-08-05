#pragma once
#include <dctl/successor/value.hpp>     // Value

namespace dctl {
namespace pool {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;                     // 13
        static constexpr auto is_jump_precedence = false;                       // 20
        static constexpr auto is_long_ranged_king = true;                       // 15

        using value_type = successor::Value;

        // draw
        static constexpr auto max_repetitions = 3;                              // 26
        static constexpr auto max_reversible_moves = 30;                        // 26
};

}       // namespace pool
}       // namespace dctl
