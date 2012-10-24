#pragma once
#include <dctl/rules/Lineo_fwd.hpp>
#include <dctl/rules/Rules.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/capture/Lineo.hpp>

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
