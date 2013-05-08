#pragma once
#include <cstdint>
#include <functional>                   // function
#include <dctl/bit/bit_fwd.hpp>         // first::clear, is_element, singlet

#ifdef _MSC_VER
#include <intrin.h>
//#pragma intrinsic(__popcnt64)
#pragma intrinsic(_BitScanForward64)
#endif

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<typename T>
struct count;

template<>
struct count<uint64_t>
{
        std::size_t operator()(uint64_t b)
        {
#ifdef _MSC_VER
                return __popcnt64(b);
#else
                return __builtin_popcountll(b);
#endif
        }
};

template<typename T>
struct index;

template<>
struct index<uint64_t>
{
        int operator()(uint64_t b)
        {
#ifdef _MSC_VER
                unsigned long index;
                _BitScanForward64(&index, b);
                return static_cast<int>(index);
#else
                return __builtin_ctzll(b);
#endif
        }
};

// TODO: partial specializations for bit arrays

}       // namespace detail

template<typename T>
std::size_t count(T b)
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
