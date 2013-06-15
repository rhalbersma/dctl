#pragma once
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template
<
        bool, class, class, class Position,
        class Range = typename rules::range::king<typename Position::rules_type>::type
>
struct detect;

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
