#pragma once

namespace dctl {
namespace rules {

template<class Rules>
struct precedence_jump
:
        Rules::precedence_jump
{};

}       // namespace rules
}       // namespace dctl
