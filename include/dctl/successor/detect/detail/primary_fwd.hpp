#pragma once
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template
<
        bool, class, class, class Position,
        class Range = typename rules::range::king<typename Position::rules_type>::type
>
struct Detect;

}       // namespace detail
}       // namespace successor
}       // namespace dctl
