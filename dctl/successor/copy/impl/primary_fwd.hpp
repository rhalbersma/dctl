#pragma once

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template
<
        bool,           // Color
        typename,       // Material
        typename,       // Select
        typename,       // Position
        typename        // Vector
>
struct copy;

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
