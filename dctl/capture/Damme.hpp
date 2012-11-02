#pragma once
#include <dctl/capture/value_fwd.hpp>           // Value (primary template)
#include <dctl/capture/international.hpp>       // International (partial specialization)
#include <dctl/rules/damme_fwd.hpp>             // Damme

namespace dctl {
namespace capture {

// partial specialization for A.K.W. Damme's proposal
template<typename Board>
struct Value<rules::Damme, Board>
:
        public Value<rules::International, Board>
{};

}       // namespace capture
}       // namespace dctl
