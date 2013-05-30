#pragma once
#include <type_traits>
#include <dctl/rules/variants/czech_fwd.hpp>
#include <dctl/rules/types.hpp>
#include <dctl/successor/value.hpp>
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
