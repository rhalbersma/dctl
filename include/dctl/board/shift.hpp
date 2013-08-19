#pragma once
#include <cstddef>                      // size_t
#include <dctl/angle/traits.hpp>

namespace dctl {
namespace {     // anonymous

// left and right direction

static constexpr auto L = false;
static constexpr auto R = true;

}               // anonymous

// primary template
template<bool>
struct Shift;

// explicit specialization for bitwise shift-left
template<>
struct Shift<L>
{
        template<class T>
        constexpr T operator()(T square, std::size_t n) const
        {
                return square << n;
        }
};

// explicit specialization for bitwise shift-right
template<>
struct Shift<R>
{
        template<class T>
        constexpr T operator()(T square, std::size_t n) const
        {
                return square >> n;
        }
};

// primary template
template<bool>
struct ShiftAssign;

// explicit specialization for bitwise shift-left assignment
template<>
struct ShiftAssign<L>
{
        template<class T>
        void operator()(T& square, std::size_t n) const
        {
                square <<= n;
        }
};

// explicit specialization for bitwise shift-right assignment
template<>
struct ShiftAssign<R>
{
        template<class T>
        void operator()(T& square, std::size_t n) const
        {
                square >>= n;
        }
};

}       // namespace dctl
