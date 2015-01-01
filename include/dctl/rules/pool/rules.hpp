#pragma once
#include <dctl/rule_traits.hpp>

namespace dctl {
namespace pool {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;     // 13
        using king_range_category = long_ranged_tag;            // 15

        // draw
        static constexpr auto max_repetitions = 3;              // 26
        static constexpr auto max_reversible_moves = 30;        // 26
};

}       // namespace pool
}       // namespace dctl
