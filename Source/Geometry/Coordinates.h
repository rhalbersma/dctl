#pragma once

namespace Geometry {
namespace Coordinates {

template<int, int>
struct Pair;

template<typename, typename, size_t> 
struct Rotate;

template<typename, int>
class FromRange;

template<typename, typename>
class ToRange;

}       // namespace Coordinates
}       // namespace Geometry

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Coordinates.hpp"
