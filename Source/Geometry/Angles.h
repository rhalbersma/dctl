#pragma once
#include <cstddef>

namespace Geometry {
namespace Angles {

// angles are subject to arithmetic modulo 360 degrees, with the unit element equal to 45 degrees anti-clockwise
 enum {
        D000 = 0, 
        D045 = 1, 
        D090 = 2, 
        D135 = 3, 
        D180 = 4, 
        D225 = 5, 
        D270 = 6, 
        D315 = 7, 
        D360 = 8 
};

template<size_t, size_t>
struct Rotate;

template<size_t>
struct Inverse;

}       // namespace Angles
}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Angles.hpp"
