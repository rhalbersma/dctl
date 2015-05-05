#pragma once

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php

struct Checkers
{
        // draw
        static constexpr auto max_repetitions = 3;              // 1.32.1
        static constexpr auto max_reversible_moves = 80;        // 1.32.2
};

}       // namespace rules
}       // namespace dctl
