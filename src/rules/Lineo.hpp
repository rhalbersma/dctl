#pragma once
#include "Lineo.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Lineo.hpp"

namespace dctl {
namespace rules {

struct Lineo
:
        Rules<
                Lineo,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::quantity>,
                king_move_orthogonality<orthogonality::relative>
        >
{};

}       // namespace rules
}       // namespace dctl
