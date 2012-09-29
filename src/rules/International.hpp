#pragma once
#include "International_fwd.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/International.hpp"

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International
:
        Rules<
                International,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::quantity>
        >
{};

}       // namespace rules
}       // namespace dctl
