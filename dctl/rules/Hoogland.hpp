#pragma once
#include <dctl/rules/Hoogland_fwd.hpp>
#include <dctl/rules/Rules.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/capture/Hoogland.hpp>

namespace dctl {
namespace rules {

// H. Hoogland's proposal
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
