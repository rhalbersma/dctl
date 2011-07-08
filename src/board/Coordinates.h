#pragma once

namespace board {

template<typename G, int R, int C>
struct Coordinates
{
        typedef G grid;
        enum { row = R, col = C };
};

template<typename G, int SQ>
struct Square
{
        typedef G grid;
        enum { square = SQ };
};

template<typename>
class Square2Coordinates;

template<typename>
class Coordinates2Square;

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Coordinates.hpp"
