#pragma once
#include <dctl/rules/variants/spanish_fwd.hpp>  // Spanish
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // quality
#include <dctl/successor/value/spanish.hpp>     // Value (Spanish specialization)

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Spanish
{
        // main rules
        using is_long_ranged_king = std::true_type;
        using is_backward_pawn_jump = std::false_type;
        using precedence_jump = precedence::quality;
};

}       // namespace rules
}       // namespace dctl
