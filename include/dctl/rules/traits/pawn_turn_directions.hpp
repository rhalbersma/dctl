#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/rules/traits/detail/turn_directions.hpp>
#include <dctl/rules/traits/pawn_jump_directions.hpp>

namespace dctl {
namespace rules {

template<typename Rules>
struct pawn_turn_directions
:
        turn_directions<
                pawn_jump_directions<Rules>
        >
{};

}       // namespace rules
}       // namespace dctl
