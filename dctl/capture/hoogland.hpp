#pragma once
#include <dctl/capture/value_fwd.hpp>                // Value (primary template)
#include <dctl/capture/international.hpp>            // International (partial specialization)
#include <dctl/rules/hoogland_fwd.hpp>    // Hoogland

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
