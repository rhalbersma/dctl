#pragma once

namespace dctl {
namespace rules {

template<typename Rules>
struct pawn_jump_directions
:
        Rules::pawn_jump_directions
{};

}       // namespace rules
}       // namespace dctl
