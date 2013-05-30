#pragma once

namespace dctl {
namespace rules {
namespace precedence {

template<class Rules>
struct jump
:
        Rules::precedence_jump
{};

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
