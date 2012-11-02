#pragma once
#include <dctl/rules/hoogland_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/hoogland.hpp>

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
