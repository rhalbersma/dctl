#pragma once
#include <dctl/rules/traits/turn_directions.hpp>
#include <dctl/rules/traits/king_jump_directions.hpp>

namespace dctl {
namespace rules {

template<typename Rules>
struct king_turn_directions
:
        turn_directions<typename
                king_jump_directions<Rules>::type
        >::type
{};

}       // namespace rules
}       // namespace dctl
