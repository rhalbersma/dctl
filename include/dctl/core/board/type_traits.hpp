#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/util/tti.hpp>    // DCTL_PP_TTI_CONSTANT
#include <algorithm>            // min, max

namespace dctl::core {

DCTL_PP_TTI_CONSTANT(width, 8)
DCTL_PP_TTI_CONSTANT(height, 8)
DCTL_PP_TTI_CONSTANT(is_inverted, false)
DCTL_PP_TTI_CONSTANT(is_orthogonal_jump, false)

template<class Geometry>
constexpr auto is_placeable_v = std::min(width_v<Geometry>, height_v<Geometry>) >= 1 && (!is_inverted_v<Geometry> || std::max(width_v<Geometry>, height_v<Geometry>) > 1);

template<class Geometry>
constexpr auto is_pushable_v = std::min(width_v<Geometry>, height_v<Geometry>) >= 2;

template<class Geometry>
constexpr auto is_jumpable_v = std::min(width_v<Geometry>, height_v<Geometry>) >= 3 && (!is_inverted_v<Geometry> || std::max(width_v<Geometry>, height_v<Geometry>) > 3);

}       // namespace dctl::core
