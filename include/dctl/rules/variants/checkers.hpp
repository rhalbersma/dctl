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
        typedef range::distance_1 range_king;                           // 1.17
        typedef directions::up directions_pawn_jump;                    // 1.18
        typedef precedence::none precedence_jump;                       // 1.20

        // drawing rules
        typedef std::integral_constant<int,  3> max_repetitions;        // 1.32.1
        typedef std::integral_constant<int, 80> max_reversible_moves;   // 1.32.2
};

}       // namespace rules
}       // namespace dctl
