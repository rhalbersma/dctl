#pragma once
#include <dctl/angle/angle.hpp> // Angle
#include <type_traits>          // integral_constant

namespace dctl {

template<int N>
using angle_t = std::integral_constant<int, Angle{N}>;

template<int N>
static constexpr auto angle_v = angle_t<N>::value;

}       // namespace dctl
