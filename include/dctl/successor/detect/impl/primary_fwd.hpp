#pragma once

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template
<
        bool, typename, typename, typename Position,
        typename Range = typename Position::rules_type::range_king
>
struct detect;

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
