#pragma once
#include <dctl/rules/variants/killer_fwd.hpp>           // Killer
#include <dctl/rules/variants/international.hpp>        // International
#include <dctl/rules/types/range.hpp>                   // distance_1K
#include <dctl/successor/value/killer.hpp>              // Value (Killer specialization)

namespace dctl {
namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules

struct Killer
:
        // main rules
        International
{
        // additional rules
        typedef range::distance_1K range_halt;
};

}       // namespace rules
}       // namespace dctl
