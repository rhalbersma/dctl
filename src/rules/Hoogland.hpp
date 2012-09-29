#pragma once
#include "Hoogland_fwd.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Hoogland.hpp"

namespace dctl {
namespace rules {

struct Hoogland
:
        Rules<
                Hoogland,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::quantity>,
                king_jump_orthogonality<orthogonality::relative>
        >
{};

}       // namespace rules
}       // namespace dctl
