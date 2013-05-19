#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/rules/traits/turn_directions.hpp>
#include <dctl/rules/traits/pawn_jump_directions.hpp>

namespace dctl {
namespace rules {

template<typename Rules>
struct pawn_turn_directions
:
        turn_directions<typename
                pawn_jump_directions<Rules>::type
        >::type
{};

}       // namespace rules
}       // namespace dctl
