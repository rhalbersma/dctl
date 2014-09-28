#pragma once

namespace dctl {
namespace util {

// primary template
template<bool>
struct shift;

// explicit specialization for bitwise shift-left
template<>
struct shift<false>
{
        template<class T>
        constexpr auto operator()(T const& t, std::size_t n) const
        {
                return t << n;
        }
};

// explicit specialization for bitwise shift-right
template<>
struct shift<true>
{
        template<class T>
        constexpr auto operator()(T const& t, std::size_t n) const
        {
                return t >> n;
        }
};

// primary template
template<bool>
struct shift_assign;

// explicit specialization for bitwise shift-left assignment
template<>
struct shift_assign<false>
{
        template<class T>
        constexpr void operator()(T& t, std::size_t n) const
        {
                t <<= n;
        }
};

// explicit specialization for bitwise shift-right assignment
template<>
struct shift_assign<true>
{
        template<class T>
        constexpr void operator()(T& t, std::size_t n) const
        {
                t >>= n;
        }
};

}       // namespace util
}       // namespace dctl
