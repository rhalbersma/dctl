#pragma once
#include <dctl/successor/value_fwd.hpp>                 // Value (primary template)
#include <dctl/successor/value/international.hpp>       // International (specialization)
#include <dctl/rules/lineo_fwd.hpp>                     // Lineo

namespace dctl {
namespace successor {

// specialization for Lineo draughts
template<>
struct Value<rules::Lineo>
:
        Value<rules::International>
{};

}       // namespace successor
}       // namespace dctl
