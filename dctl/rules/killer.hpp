#pragma once
#include <dctl/rules/killer_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/killer.hpp>

namespace dctl {
namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct Killer
:
        Rules<
                Killer,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::quantity>,
                halt_range<range::distance_1K>
        >
{};

}       // namespace rules
}       // namespace dctl
