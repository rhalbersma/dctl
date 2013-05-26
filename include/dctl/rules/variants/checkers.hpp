#pragma once
#include <dctl/rules/variants/checkers_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value.hpp>

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php

struct Checkers
{
        // main rules
        typedef range::distance_1 range_king;                           // 1.17
        typedef directions::up directions_pawn_jump;                    // 1.18
        typedef precedence::none precedence_jump;                       // 1.20

        // drawing rules
        typedef std::integral_constant<int,  3> max_repetitions;        // 1.32.1
        typedef std::integral_constant<int, 80> max_reversible_moves;   // 1.32.2
};

}       // namespace rules
}       // namespace dctl
