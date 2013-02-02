#pragma once

namespace dctl {
namespace successor {
namespace detail {

template
<
        bool, int, typename, typename Position,
        typename Range = typename Position::rules_type::king_range
>
struct detect;

}       // namespace detail
}       // namespace successor
}       // namespace dctl
