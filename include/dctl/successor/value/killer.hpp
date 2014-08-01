#pragma once
#include <dctl/successor/value_fwd.hpp> // Value (primary template)
#include <dctl/rules/killer_fwd.hpp>    // Killer

namespace dctl {
namespace successor {

// partial specialization for Killer draughts
template<class Rules>
class Value<rules::Killer<Rules>>
:
        public Value<Rules>
{
        using Value<Rules>::Value;
};

}       // namespace successor
}       // namespace dctl
