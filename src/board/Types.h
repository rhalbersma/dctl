#pragma once
#include "Angle.h"
#include "Board.h"
#include "Dimensions.h"
#include "Structure.h"

namespace board {

// square boards
typedef Board< Dimensions< 4,  4, false> > Micro;
typedef Board< Dimensions< 6,  6, false> > Mini;
typedef Board< Dimensions< 8,  8, false> > Checkers;
typedef Board< Dimensions<10, 10, false> > International;

// Frisian board needs at least 2 ghost columns
typedef Board< Dimensions<10, 10, false>, Structure<2, 2> > Frisian;

// Spanish-Italian board has opposite coloring
typedef Board< Dimensions< 8,  8, true> > Roman;

// Thai board has 4 demilitarized rows in the initial position
typedef Board< Dimensions< 8,  8, false>, Structure<4> > Thai;

// rectangular boards
typedef Board< Dimensions<10,  8, true>                               > Spantsireti;
typedef Board< Dimensions<11, 10, true>, Structure<3, 1>              > Ktar11;
typedef Board< Dimensions<12, 10, true>, Structure<2, 1, Angle::R090> > Ktar12;

}       // namespace board
