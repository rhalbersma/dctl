#pragma once
#include <dctl/rules/spanish_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/capture/spanish.hpp>

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Spanish
{
        // main rules
        typedef range::distance_N king_range;
        typedef directions::up pawn_jump_directions;
        typedef precedence::quality jump_precedence;
};

}       // namespace rules
}       // namespace dctl
