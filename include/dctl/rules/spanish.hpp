#pragma once
#include <dctl/rules/spanish_fwd.hpp>           // Spanish
#include <dctl/successor/value/spanish.hpp>     // Value (Spanish specialization)

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Spanish
{
        // main rules
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_jump_precedence = true;
};

}       // namespace rules
}       // namespace dctl
