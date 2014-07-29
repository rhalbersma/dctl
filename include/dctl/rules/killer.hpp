#pragma once
#include <dctl/rules/killer_fwd.hpp>            // Killer
#include <dctl/rules/international.hpp>         // International
#include <dctl/successor/value/killer.hpp>      // Value (Killer specialization)

namespace dctl {
namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules

struct Killer
:
        // main rules
        International
{
        // additional rules
        static constexpr auto is_directly_halt_after_final_king = true;
};

}       // namespace rules
}       // namespace dctl
