#pragma once
#include <dctl/rules/lineo_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/lineo.hpp>

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
