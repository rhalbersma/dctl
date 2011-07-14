#pragma once
#include "Transform.h"

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
class Coordinates2Square;

template<typename>
class Square2Coordinates;

template<typename G, int R, int C, int I>
struct Rotate<Coordinates<G, R, C>, I>;

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Coordinates.hpp"
