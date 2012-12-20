#pragma once
#include <dctl/capture/value_fwd.hpp>           // Value (primary template)
#include <dctl/capture/international.hpp>       // International (partial specialization)
#include <dctl/rules/killer_fwd.hpp>            // Killer

namespace dctl {
namespace capture {

// partial specialization for Killer draughts
template<typename Board>
struct Value<rules::Killer, Board>
:
        Value<rules::International, Board>
{};

}       // namespace capture
}       // namespace dctl
