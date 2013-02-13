#pragma once
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace successor {
namespace detail {

template
<
        bool /* Color */, typename /* Material */, typename /* Select */, typename Range = rules::range::distance_1
>
struct detect;

}       // namespace detail
}       // namespace successor
}       // namespace dctl
