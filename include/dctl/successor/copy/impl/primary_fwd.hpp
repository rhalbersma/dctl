#pragma once

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template
<
        bool,   // Color
        class,  // Material
        class,  // Select
        class,  // Position
        class   // Vector
>
struct copy;

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
