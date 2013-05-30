#pragma once
#include <dctl/rules/traits/range/scan.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template
<
        bool, typename, typename, typename Position,
        class Range = typename rules::range::scan<typename Position::rules_type>::type
>
struct detect;

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
