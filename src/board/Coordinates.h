#pragma once
#include <boost/config.hpp>
#include "Transform.h"

namespace dctl {
namespace board {

template<typename G, int R, int C>
struct Coordinates
{
        typedef G grid;
        BOOST_STATIC_CONSTANT(auto, row = R); 
        BOOST_STATIC_CONSTANT(auto, col = C);
};

template<typename G, int SQ>
struct Square
{
        typedef G grid;
        BOOST_STATIC_CONSTANT(auto, square = SQ);
};

template<typename>
class Coordinates2Square;

template<typename>
class Square2Coordinates;

}       // namespace board

// partial specialization for coordinates
template<typename Grid, int Row, int Column, int Angle>
struct rotate<board::Coordinates<Grid, Row, Column>, Angle>;

}       // namespace dctl

// include template definitions inside header
#include "Coordinates.hpp"
