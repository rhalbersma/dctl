#pragma once

namespace board {
namespace angles {

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

template<int, int>
struct Rotate;

template<int>
struct Inverse;

}       // namespace angles
}       // namespace board

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Angles.hpp"
