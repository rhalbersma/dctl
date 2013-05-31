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
        typedef range::distance_N range_king;
        typedef directions::up directions_pawn_jump;
        typedef precedence::none precedence_jump;

        // additional rules
        typedef std::true_type precedence_is_absolute_king;
};

}       // namespace rules
}       // namespace dctl
