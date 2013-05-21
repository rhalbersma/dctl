#pragma once

namespace dctl {
namespace rules {

template<class Rules>
struct pawn_jump_directions
:
        Rules::pawn_jump_directions
{};

}       // namespace rules
}       // namespace dctl
