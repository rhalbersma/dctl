#pragma once

namespace dctl {
namespace rules {

template<class Rules>
struct king_range
:
        Rules::king_range
{};

}       // namespace rules
}       // namespace dctl
