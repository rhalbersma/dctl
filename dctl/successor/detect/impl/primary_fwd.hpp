#pragma once

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template
<
        bool, int, typename, typename Position,
        typename Range = typename Position::rules_type::king_range
>
struct detect;

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
