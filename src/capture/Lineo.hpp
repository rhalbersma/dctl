#pragma once
#include "Value_fwd.hpp"                // Value (primary template)
#include "International.hpp"            // International (partial specialization)
#include "../rules/Lineo_fwd.hpp"       // Lineo

namespace dctl {
namespace capture {

// partial specialization for Lineo draughts
template<typename Board>
struct Value<rules::Lineo, Board>
:
        public Value<rules::International, Board>
{};

}       // namespace capture
}       // namespace dctl
