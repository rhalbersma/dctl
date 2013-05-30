#pragma once
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/traits/directions/detail/turn.hpp> // turn

namespace dctl {
namespace rules {
namespace directions {

template<class Rules>
struct pawn_turn
:
        detail::turn< pawn_jump<Rules> >
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
