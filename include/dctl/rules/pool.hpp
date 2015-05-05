#pragma once
#include <dctl/rule_traits.hpp> // long_ranged_tag

namespace dctl {
namespace rules {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Pool
{
        static constexpr auto is_backward_pawn_jump = true;     // 13
        using king_range_category = long_ranged_tag;            // 15

        static constexpr auto max_repetitions = 3;              // 26
        static constexpr auto max_reversible_moves = 30;        // 26
};

}       // namespace rules
}       // namespace dctl
