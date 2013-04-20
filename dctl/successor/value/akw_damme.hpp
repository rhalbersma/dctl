#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/variants/akw_damme_fwd.hpp>        // AKWDamme

namespace dctl {
namespace successor {

// specialization for A.K.W. Damme's proposal
template<>
struct Value<rules::AKWDamme>
:
        Value<rules::International>
{};

}       // namespace successor
}       // namespace dctl
