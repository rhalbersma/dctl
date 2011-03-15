#pragma once
#include <cstddef>

namespace Geometry {
namespace Direction {

template<typename, bool> 
class Indices;

template<size_t>
class Traits;

template<size_t>
struct Rotate;

template<size_t>
struct MirrorUp;

template<size_t>
struct MirrorDown;

}       // namespace Direction
}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Direction.hpp"
