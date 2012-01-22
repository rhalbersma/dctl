#pragma once
#include "Degrees.hpp"
#include "Board.h"
#include "Dimensions.hpp"
#include "Structure.hpp"

namespace dctl {
namespace board {

// square boards
typedef Board< Dimensions< 4,  4> > Micro;
typedef Board< Dimensions< 6,  6> > Mini;
typedef Board< Dimensions< 8,  8> > Checkers;
typedef Board< Dimensions<10, 10> > International;

// the Spanish-Italian board has opposite coloring
typedef Board< Dimensions< 8,  8, true> > Roman;

// the Thai board has 4 demilitarized rows in the initial position
typedef Board< Dimensions< 8,  8>, Structure<4> > Thai;

// the Frisian board needs at least 2 ghost columns
// in order to accomodate orthogonal captures
typedef Board< Dimensions<10, 10>, Structure<2, 2> > Frisian;

// rectangular boards always have opposite coloring,
// can have at most 1 ghost column when larger than 10x10,
// and require internal rotations for 12x10 to fit within 64-bits
typedef Board< Dimensions<10,  8, true>                                         > Spantsireti;
typedef Board< Dimensions<11, 10, true>, Structure<3, 1>                        > Ktar11;
typedef Board< Dimensions<12, 10, true>, Structure<2, 1, angle<degrees::R090> > > Ktar12;

}       // namespace board
}       // namespace dctl
