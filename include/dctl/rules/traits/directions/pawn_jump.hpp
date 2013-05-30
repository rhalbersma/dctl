#pragma once

namespace dctl {
namespace rules {
namespace directions {

template<class Rules>
struct pawn_jump
:
        Rules::directions_pawn_jump
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
