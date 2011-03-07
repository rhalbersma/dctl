#pragma once
#include <cstddef>

namespace Geometry {
namespace Direction {

template<typename, bool> 
class Indices;

template<size_t>
struct Rotate;

template<size_t>
struct MirrorForward;

template<size_t>
class Traits;

}       // namespace Direction
}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Direction.hpp"
