#pragma once
#include <cstdint>
#include <dctl/bit/bit_fwd.hpp>         // first::clear, is_element, singlet

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<class T>
struct count;

template<>
struct count<uint32_t>
{
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_popcountl(b));
        }
};

template<>
struct count<uint64_t>
{
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_popcountll(b));
        }
};

template<class T>
struct find;

template<>
struct find<uint32_t>
{
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_ctzl(b));
        }
};

template<>
struct find<uint64_t>
{
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_ctzll(b));
        }
};

// TODO: partial specializations for bit arrays

}       // namespace detail

template<class T>
int count(T b)
{
        return detail::count<T>()(b);
}

template<class T>
int find(T b)
{
        return detail::find<T>()(b);
}

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
