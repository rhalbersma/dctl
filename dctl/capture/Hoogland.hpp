#pragma once
#include <dctl/capture/Value_fwd.hpp>                // Value (primary template)
#include <dctl/capture/International.hpp>            // International (partial specialization)
#include <dctl/rules/Hoogland_fwd.hpp>    // Hoogland

namespace dctl {
namespace capture {

// partial specialization for H. Hoogland's proposal
template<typename Board>
struct Value<rules::Hoogland, Board>
:
        public Value<rules::International, Board>
{};

}       // namespace capture
}       // namespace dctl
