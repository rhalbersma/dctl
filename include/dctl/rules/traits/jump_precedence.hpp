#pragma once

namespace dctl {
namespace rules {

template<typename Rules>
struct jump_precedence
:
        Rules::jump_precedence
{};

}       // namespace rules
}       // namespace dctl
