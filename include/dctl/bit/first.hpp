#pragma once

namespace dctl {
namespace bit {
namespace first {
namespace detail {

template<class T>
struct not_equal_to
{
        T operator()(T b) const
        {
                return b & (b - T(1));
        }
};

}       // namespace detail

// most significant 1-bits
template<class T>
T not_equal_to(T b)
{
        return detail::not_equal_to<T>()(b);
}

}       // namespace first
}       // namespace bit
}       // namespace dctl
