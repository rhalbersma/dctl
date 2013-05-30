#pragma once
#include <dctl/rules/traits/directions/king_jump.hpp>   // king_jump
#include <dctl/rules/traits/directions/detail/turn.hpp> // turn

namespace dctl {
namespace rules {
namespace directions {

template<class Rules>
struct king_turn
:
        detail::turn< king_jump<Rules> >
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
