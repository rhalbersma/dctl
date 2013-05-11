#pragma once
#include <cstdint>
#include <dctl/bit/bit_fwd.hpp>         // first::clear, is_element, singlet

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<typename T>
struct count;

template<>
struct count<uint64_t>
{
        int operator()(uint64_t b)
        {
                return static_cast<int>(__builtin_popcountll(b));
        }
};

template<typename T>
struct index;

template<>
struct index<uint64_t>
{
        int operator()(uint64_t b)
        {
                return static_cast<int>(__builtin_ctzll(b));
        }
};

// TODO: partial specializations for bit arrays

}       // namespace detail

template<typename T>
int count(T b)
{
        return detail::count<T>()(b);
}

template<typename T>
int index(T b)
{
        return detail::index<T>()(b);
}

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
