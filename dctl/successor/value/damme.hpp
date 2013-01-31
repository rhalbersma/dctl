#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/damme_fwd.hpp>                     // Damme

namespace dctl {
namespace successor {

// specialization for A.K.W. Damme's proposal
template<>
struct Value<rules::Damme>
:
        Value<rules::International>
{};

}       // namespace successor
}       // namespace dctl
