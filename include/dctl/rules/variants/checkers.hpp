#pragma once
#include <dctl/rules/variants/checkers_fwd.hpp> // Checkers
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (primary template)
#include <type_traits>                          // integral_constant, false_type

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php

struct Checkers
{
        // main rules

        using is_long_ranged_king = std::false_type;                    // 1.17
        using is_backward_pawn_jump = std::false_type;                  // 1.18
        using precedence_jump = precedence::none;                       // 1.20

        // drawing rules

        using max_repetitions = std::integral_constant<int,  3>;        // 1.32.1
        using max_reversible_moves = std::integral_constant<int, 80>;   // 1.32.2
};

}       // namespace rules
}       // namespace dctl
