#pragma once

namespace dctl {
namespace checkers {

// http://www.usacheckers.com/rulesofcheckers.php

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;    // 1.18
        static constexpr auto is_long_ranged_king = false;      // 1.17

        // draw
        static constexpr auto max_repetitions = 3;              // 1.32.1
        static constexpr auto max_reversible_moves = 80;        // 1.32.2
};

}       // namespace checkers
}       // namespace dctl
