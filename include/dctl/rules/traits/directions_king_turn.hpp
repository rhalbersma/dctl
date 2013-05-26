#pragma once
#include <dctl/rules/traits/detail/directions_turn.hpp>
#include <dctl/rules/traits/directions_king_jump.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct directions_king_turn
:
        directions_turn<
                directions_king_jump<Rules>
        >
{};

}       // namespace rules
}       // namespace dctl
