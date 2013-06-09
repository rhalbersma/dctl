#pragma once

namespace dctl {
namespace bit {
namespace first {
namespace detail {

template<typename T>
struct equal
{
        T operator()(T b) const
        {
                return b & (T(0) - b);
        }
};

template<typename T>
struct not_equal_to
{
        T operator()(T b) const
        {
                return b & (b - T(1));
        }
};

template<typename T>
struct less
{
        T operator()(T b) const
        {
                return ~b & (b - T(1));
        }
};

template<typename T>
struct less_equal
{
        T operator()(T b) const
        {
                return b ^ (b - T(1));
        }
};


// TODO: partial specializations for bit arrays

}       // namespace detail

// least significant 1-bit
template<typename T>
T equal(T b)
{
        return detail::equal<T>()(b);
}

// most significant 1-bits
template<typename T>
T not_equal_to(T b)
{
        return detail::not_equal_to<T>()(b);
}

}       // namespace first
}       // namespace bit
}       // namespace dctl
