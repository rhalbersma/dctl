#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/killer_fwd.hpp>                    // Killer

namespace dctl {
namespace successor {

// specialization for Killer draughts
template<>
struct Value<rules::Killer>
:
        Value<rules::International>
{};

}       // namespace successor
}       // namespace dctl
