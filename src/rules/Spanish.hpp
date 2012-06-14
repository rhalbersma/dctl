#pragma once
#include "Rules.hpp"
#include "Enum.hpp"

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct Spanish
: 
        Rules<
                king_range<range::distance_N>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::quality>
        > 
{};

}       // namespace rules
}       // namespace dctl

#include "../capture/Spanish.hpp"
