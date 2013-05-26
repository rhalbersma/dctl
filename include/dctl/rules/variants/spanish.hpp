#pragma once
#include <dctl/rules/variants/spanish_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/spanish.hpp>

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Spanish
{
        // main rules
        typedef range::distance_N range_king;
        typedef directions::up directions_pawn_jump;
        typedef precedence::quality precedence_jump;
};

}       // namespace rules
}       // namespace dctl
