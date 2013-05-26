#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/rules/traits/detail/directions_turn.hpp>
#include <dctl/rules/traits/directions_pawn_jump.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct directions_pawn_turn
:
        directions_turn<
                directions_pawn_jump<Rules>
        >
{};

}       // namespace rules
}       // namespace dctl
