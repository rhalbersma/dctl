#pragma once
#include <dctl/rules/traits/detail/turn_directions.hpp>
#include <dctl/rules/traits/king_jump_directions.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct king_turn_directions
:
        turn_directions<
                king_jump_directions<Rules>
        >
{};

}       // namespace rules
}       // namespace dctl
