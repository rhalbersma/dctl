#pragma once
#include <dctl/angle/degrees.hpp>       // R090
#include <dctl/board/board.hpp>         // Board
#include <dctl/board/dimensions.hpp>    // Dimensions
#include <dctl/board/ghosts.hpp>        // Ghosts

namespace dctl {
namespace board {

// square boards
typedef Board< Dimensions< 4,  4> > Micro;
typedef Board< Dimensions< 6,  6> > Mini;
typedef Board< Dimensions< 8,  8> > Checkers;
typedef Board< Dimensions<10, 10> > International;

// the Spanish-Italian board has opposite coloring
typedef Board< Dimensions< 8,  8, true> > Roman;

// the Frisian board needs at least 2 ghost columns
// in order to accomodate orthogonal captures
typedef Board< Dimensions<10, 10>, Ghosts<2> > Frisian;

// rectangular boards always have opposite coloring,
// can have at most 1 ghost column when larger than 10x10,
// and require internal rotations for 12x10 to fit within 64-bits
typedef Board< Dimensions<10,  8, true>                         > Spantsireti;
typedef Board< Dimensions<11, 10, true>, Ghosts<1>              > Ktar11;
typedef Board< Dimensions<12, 10, true>, Ghosts<1, angle::R090> > Ktar12;

}       // namespace board
}       // namespace dctl