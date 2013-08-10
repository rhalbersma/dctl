#pragma once
#include <dctl/angle/degrees.hpp>       // R090
#include <dctl/board/board.hpp>         // Board
#include <dctl/board/dimensions.hpp>    // Dimensions
#include <dctl/board/ghosts.hpp>        // Ghosts

namespace dctl {
namespace board {

// square boards
using Micro = Board< Dimensions< 4,  4> >;
using Mini = Board< Dimensions< 6,  6> >;
using Checkers = Board< Dimensions< 8,  8> >;
using International = Board< Dimensions<10, 10> >;

// the Spanish-Italian board has opposite coloring
using Roman = Board< Dimensions< 8,  8, true> >;

// the Frisian board needs at least 2 ghost columns
// in order to accomodate orthogonal captures
using Frisian = Board< Dimensions<10, 10>, Ghosts<2> >;

// rectangular boards always have opposite coloring,
// can have at most 1 ghost column when larger than 10x10,
// and require internal rotations for 12x10 to fit within 64-bits
using Spantsireti  = Board< Dimensions<10,  8, true> > ;
using Ktar11 = Board< Dimensions<11, 10, true>, Ghosts<1> >;
using Ktar12 = Board< Dimensions<12, 10, true>, Ghosts<1, angle::R090> >;

}       // namespace board
}       // namespace dctl
