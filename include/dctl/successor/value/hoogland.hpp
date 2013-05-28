#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/variants/hoogland_fwd.hpp>         // Hoogland

namespace dctl {
namespace successor {

// specialization for H. Hoogland's proposal
template<>
class Value<rules::Hoogland>
:
        public Value<rules::International>
{
        using Value<rules::International>::Value;
};

}       // namespace successor
}       // namespace dctl
