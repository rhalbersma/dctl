#pragma once
#include <dctl/rules/czech_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/value.hpp>
#include <dctl/notation/czech.hpp>

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
