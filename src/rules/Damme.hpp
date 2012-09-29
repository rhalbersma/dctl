#pragma once
#include "Damme_fwd.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Damme.hpp"

namespace dctl {
namespace rules {

// A.K.W. Damme's proposal
struct Damme
:
        Rules<
                Damme,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::quantity>,
                halt_range<range::distance_1>
        >
{};

}       // namespace rules
}       // namespace dctl
