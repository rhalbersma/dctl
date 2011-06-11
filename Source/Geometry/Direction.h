#pragma once

namespace geometry {
namespace direction {

template<typename, bool> 
class Indices;

template<int>
class Traits;

template<int>
struct Rotate;

template<int>
struct MirrorUp;

template<int>
struct MirrorDown;

}       // namespace direction
}       // namespace geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Direction.hpp"
