#pragma once
#include <dctl/rules/Czech_fwd.hpp>
#include <dctl/rules/Rules.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/capture/Value.hpp>
#include <dctl/notation/Czech.hpp>

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech
:
        Rules<
                Czech,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::none>,
                is_absolute_king_precedence<boost::mpl::true_>
        >
{};

}       // namespace rules
}       // namespace dctl
