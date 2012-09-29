#pragma once
#include "Value_fwd.hpp"                // Value (primary template)
#include "International.hpp"            // International (partial specialization)
#include "../rules/Hoogland_fwd.hpp"    // Hoogland

namespace dctl {
namespace capture {

// partial specialization for Lineo draughts
template<typename Board>
struct Value<rules::Hoogland, Board>
:
        public Value<rules::International, Board>
{};

}       // namespace capture
}       // namespace dctl
