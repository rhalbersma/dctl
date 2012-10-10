#pragma once
#include "Transform.h"

namespace dctl {
namespace board {

template<typename G, int R, int C>
struct Coordinates
{
        typedef G grid;
        enum { 
                row = R, 
                col = C 
        };
};

template<typename G, int SQ>
struct Square
{
        typedef G grid;
        enum { 
                square = SQ 
        };
};

template<typename>
class Coordinates2Square;

template<typename>
class Square2Coordinates;

// partial specialization for coordinates
template<typename G, int R, int C, int I>
struct Rotate<Coordinates<G, R, C>, I>;

}       // namespace board
}       // namespace dctl

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Coordinates.hpp"
