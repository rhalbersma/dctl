#pragma once
#include <dctl/capture/Value_fwd.hpp>                // Value (primary template)
#include <dctl/capture/International.hpp>            // International (partial specialization)
#include <dctl/rules/Lineo_fwd.hpp>       // Lineo

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
