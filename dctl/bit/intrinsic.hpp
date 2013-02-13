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
                __asm__("popcnt %1, %0" : "=r" (b) : "r" (b));
                return b;
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
                uint64_t index;
                __asm__("bsfq %1, %0": "=r"(index): "rm"(b) );
                return static_cast<int>(index);
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
