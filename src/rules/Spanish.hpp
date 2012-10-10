#pragma once
#include "Spanish_fwd.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Spanish.hpp"

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct Spanish
:
        Rules<
                Spanish,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::quality>
        >
{};

}       // namespace rules
}       // namespace dctl
