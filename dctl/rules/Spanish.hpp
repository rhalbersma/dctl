#pragma once
#include <dctl/rules/spanish_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/spanish.hpp>

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
