#pragma once
#include <dctl/rules/killer_fwd.hpp>
#include <dctl/rules/international.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/killer.hpp>

namespace dctl {
namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules

struct Killer
:
        // main rules
        International
{
        // additional rules
        typedef range::distance_1K halt_range;
};

}       // namespace rules
}       // namespace dctl
