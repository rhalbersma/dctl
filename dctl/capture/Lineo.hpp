#pragma once
#include <dctl/capture/value_fwd.hpp>           // Value (primary template)
#include <dctl/capture/international.hpp>       // International (partial specialization)
#include <dctl/rules/lineo_fwd.hpp>             // Lineo

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
