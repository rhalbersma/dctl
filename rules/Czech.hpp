#pragma once
#include "Rules.hpp"
#include "Enum.hpp"

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech
: 
        Rules<
                king_range<range::distance_N>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::none>,
                is_absolute_king_precedence<boost::mpl::true_>
        > 
{};

}       // namespace rules
}       // namespace dctl

#include "../notation/Czech.hpp"
