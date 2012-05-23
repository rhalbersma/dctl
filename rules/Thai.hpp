#pragma once
#include "Rules.hpp"
#include "Enum.hpp"

namespace dctl {
namespace rules {

struct Thai
: 
        Rules<
                king_range<range::distance_N>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::none>,
                land_range<range::distance_1>,
                is_jump_direction_reversal<boost::mpl::true_>,
                jump_removal<removal::en_passant>
        > 
{};

}       // namespace rules
}       // namespace dctl

#include "../notation/Thai.hpp"
