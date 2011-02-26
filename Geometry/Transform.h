#pragma once
#include <cstddef>

namespace Geometry {

template<size_t, size_t>
struct AntiClockwise;

template<size_t>
struct Inverse;

template<size_t>
struct Mirror;

template<size_t>
struct Rotate;

template<typename, int, int, size_t> 
struct RotateCoordinates;

template<typename, size_t> 
struct RotateSquares;

}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Transform.hpp"
