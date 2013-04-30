#pragma once
#include <type_traits>
#include <dctl/rules/variants/czech_fwd.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/successor/value.hpp>
#include <dctl/notation/czech.hpp>

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html

struct Czech
{
        // main rules
        typedef range::distance_N king_range;
        typedef directions::up pawn_jump_directions;
        typedef precedence::none jump_precedence;

        // additional rules
        typedef std::true_type is_absolute_king_precedence;
};

}       // namespace rules
}       // namespace dctl
