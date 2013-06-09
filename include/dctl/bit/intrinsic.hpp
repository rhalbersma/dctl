#pragma once
#include <cstdint>                      // uint32_t, uint64_t

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<class T>
struct front;

template<>
struct front<uint32_t>
{
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_ctzl(b));
        }
};

template<>
struct front<uint64_t>
{
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_ctzll(b));
        }
};

template<class T>
struct back;

template<>
struct back<uint32_t>
{
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_clzl(b));
        }
};

template<>
struct back<uint64_t>
{
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_clzll(b));
        }
};

template<class T>
struct size;

template<>
struct size<uint32_t>
{
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_popcountl(b));
        }
};

template<>
struct size<uint64_t>
{
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_popcountll(b));
        }
};

}       // namespace detail

template<class T>
int front(T b)
{
        return detail::front<T>()(b);
}

template<class T>
int back(T b)
{
        return detail::back<T>()(b);
}

template<class T>
int size(T b)
{
        return detail::size<T>()(b);
}

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
