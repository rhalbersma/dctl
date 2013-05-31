#pragma once
#include <dctl/rules/variants/spanish_fwd.hpp>  // Spanish
#include <dctl/rules/types/range.hpp>           // distance_N
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // quality
#include <dctl/successor/value/spanish.hpp>     // Value (Spanish specialization)

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
