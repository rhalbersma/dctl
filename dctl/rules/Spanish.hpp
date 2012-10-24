#pragma once
#include <dctl/rules/Spanish_fwd.hpp>
#include <dctl/rules/Rules.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/capture/Spanish.hpp>

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
