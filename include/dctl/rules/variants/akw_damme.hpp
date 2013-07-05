#pragma once
#include <dctl/rules/variants/akw_damme_fwd.hpp>        // AKWDamme
#include <dctl/rules/variants/international.hpp>        // International
#include <dctl/rules/types/range.hpp>                   // distance_1
#include <dctl/successor/value/akw_damme.hpp>           // Value (AKWDamme specialization)

namespace dctl {
namespace rules {

// A.K.W. Damme's proposal (1922)

struct AKWDamme
:
        // main rules
        International
{
        // additional rules
        using range_halt = range::distance_1;
};

}       // namespace rules
}       // namespace dctl
