#pragma once
#include <dctl/angle/degrees.hpp>       // Degrees
#include <dctl/angle/transform.hpp>     // inverse

namespace dctl {
namespace board {

template<int C = 2, int A = 0>
struct Ghosts
{
        static_assert(C > 0, "A board cannot have a non-positive number of ghost columns.");
        static_assert(A % 90 == 0, "A board can only be rotated in multiples of 90 degrees.");

        static constexpr auto columns = C;
        static constexpr auto full_angle = A;
        static constexpr auto inverse_angle = mpl::lazy::inverse< angle::Degrees<full_angle> >::value;
};

}       // namespace board
}       // namespace dctl
