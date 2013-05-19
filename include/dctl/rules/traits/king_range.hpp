#pragma once

namespace dctl {
namespace rules {

template<typename Rules>
struct king_range
:
        Rules::king_range
{};

}       // namespace rules
}       // namespace dctl
