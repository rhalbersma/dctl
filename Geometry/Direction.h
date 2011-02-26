#pragma once
#include <cstddef>

namespace Geometry {

template<typename, bool> 
class DirIndex;

template<size_t>
class DirTraits;

}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Direction.hpp"
