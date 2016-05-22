#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace util {

template<bool>
struct Shift;

template<>
struct Shift<false>
{
        template<class T>
        constexpr auto operator()(T const& t, std::size_t const n) const
        {
                return t << n;
        }
};

template<>
struct Shift<true>
{
        template<class T>
        constexpr auto operator()(T const& t, std::size_t const n) const
        {
                return t >> n;
        }
};

template<bool>
struct ShiftAssign;

template<>
struct ShiftAssign<false>
{
        template<class T>
        constexpr auto operator()(T& t, std::size_t const n) const
        {
                t <<= n;
        }
};

template<>
struct ShiftAssign<true>
{
        template<class T>
        constexpr auto operator()(T& t, std::size_t const n) const
        {
                t >>= n;
        }
};

}       // namespace util
}       // namespace dctl
