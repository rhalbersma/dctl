#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/variants/akw_damme_fwd.hpp>        // Damme

namespace dctl {
namespace successor {

// specialization for A.K.W. Damme's proposal
template<>
class Value<rules::Damme>
:
        public Value<rules::International>
{
        //using Value<rules::International>::Value;
};

}       // namespace successor
}       // namespace dctl
