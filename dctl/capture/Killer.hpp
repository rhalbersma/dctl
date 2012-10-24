#pragma once
#include <dctl/capture/Value_fwd.hpp>                // Value (primary template)
#include <dctl/capture/International.hpp>            // International (partial specialization)
#include <dctl/rules/Killer_fwd.hpp>      // Killer

namespace dctl {
namespace capture {

// partial specialization for Killer draughts
template<typename Board>
struct Value<rules::Killer, Board>
:
        public Value<rules::International, Board>
{};

}       // namespace capture
}       // namespace dctl
