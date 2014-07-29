#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/killer_fwd.hpp>           // Killer

namespace dctl {
namespace successor {

// specialization for Killer draughts
template<>
class Value<rules::Killer>
:
        public Value<rules::International>
{
        using Value<rules::International>::Value;
};

}       // namespace successor
}       // namespace dctl
