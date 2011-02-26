#pragma once

namespace Geometry {

template<typename, int>
class BORDER_IS_GHOST;

template<typename, int>
class BIT2COORD;

template<typename, int, int>
class COORD2BIT;

}       // namespace Geometry

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "BordersPredicates.hpp"
