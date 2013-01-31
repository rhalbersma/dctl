#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/hoogland_fwd.hpp>                  // Hoogland

namespace dctl {
namespace successor {

// specialization for H. Hoogland's proposal
template<>
struct Value<rules::Hoogland>
:
        Value<rules::International>
{};

}       // namespace successor
}       // namespace dctl
