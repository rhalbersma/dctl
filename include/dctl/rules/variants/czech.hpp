#pragma once
#include <type_traits>                          // true_type
#include <dctl/rules/variants/czech_fwd.hpp>    // Czech
#include <dctl/rules/types/range.hpp>           // distance_N
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (primary template)
#include <dctl/notation/czech.hpp>

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html

struct Czech
{
        // main rules
        using range_king = range::distance_N;
        using directions_pawn_jump = directions::up;
        using precedence_jump = precedence::none;

        // additional rules
        using precedence_is_absolute_king = std::true_type;
};

}       // namespace rules
}       // namespace dctl
