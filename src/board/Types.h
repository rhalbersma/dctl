#pragma once
#include "Angle.h"
#include "Board.h"
#include "Grid.h"

namespace board {

// square boards
typedef Board< Grid< 4,  4, false> > Micro;
typedef Board< Grid< 6,  6, false> > Mini;
typedef Board< Grid< 8,  8, false> > Checkers;
typedef Board< Grid<10, 10, false> > International;

// Frisian board needs at least 2 ghost columns
typedef Board< Grid<10, 10, false>, 2, 2 > Frisian;

// Spanish-Italian board is mirrored
typedef Board< Grid< 8,  8, true> > Roman;

// Thai board has 4 demilitarized rows in the initial position
typedef Board< Grid< 8,  8, false>, 4 > Thai;

// rectangular boards
typedef Board< Grid<10,  8, true>                    > Spantsireti;
typedef Board< Grid<11, 10, true>, 3, 1              > Ktar11;
typedef Board< Grid<12, 10, true>, 2, 1, Angle::R090 > Ktar12;

}       // namespace board
