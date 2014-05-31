#pragma once
#include <dctl/rules/variants/h_hoogland_fwd.hpp>       // HHoogland
#include <dctl/rules/variants/international.hpp>        // International
#include <dctl/rules/types/orthogonality.hpp>           // relative
#include <dctl/successor/value/h_hoogland.hpp>          // Value (HHoogland specialization)

namespace dctl {
namespace rules {

// H. Hoogland's proposal (1923)

struct HHoogland
:
        // main rules
        International
{
        // additional rules
        using orthogonality_king_jump = orthogonality::relative;
};

}       // namespace rules
}       // namespace dctl
