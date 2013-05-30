#pragma once

namespace dctl {
namespace rules {
namespace range {

template<class Rules>
struct scan
:
        Rules::range_king
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
