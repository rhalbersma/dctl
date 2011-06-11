#pragma once

namespace geometry {
namespace coordinates {

template<int, int>
struct Pair;

template<typename, typename, int> 
struct Rotate;

template<typename, int>
class FromRange;

template<typename, typename>
class ToRange;

}       // namespace coordinates
}       // namespace geometry

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Coordinates.hpp"
