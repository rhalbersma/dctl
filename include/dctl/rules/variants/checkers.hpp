#pragma once
#include <type_traits>                          // integral_constant
#include <dctl/rules/variants/checkers_fwd.hpp> // Checkers
#include <dctl/rules/types/range.hpp>           // distance_1
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (primary template)

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php

struct Checkers
{
        // main rules

        using range_king = range::distance_1;                           // 1.17
        using directions_pawn_jump = directions::up;                    // 1.18
        using precedence_jump = precedence::none;                       // 1.20

        // drawing rules

        using max_repetitions = std::integral_constant<int,  3>;        // 1.32.1
        using max_reversible_moves = std::integral_constant<int, 80>;   // 1.32.2
};

}       // namespace rules
}       // namespace dctl
