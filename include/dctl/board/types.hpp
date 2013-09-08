#pragma once
#include <dctl/angle/degrees.hpp>       // R090
#include <dctl/board/board.hpp>         // Board
#include <dctl/grid/dimensions.hpp>     // Dimensions

namespace dctl {
namespace board {

// square boards
using Micro         = Board< grid::Dimensions< 4,  4> >;
using Mini          = Board< grid::Dimensions< 6,  6> >;
using Checkers      = Board< grid::Dimensions< 8,  8> >;
using International = Board< grid::Dimensions<10, 10> >;

// the Spanish-Italian board has opposite coloring
using Roman         = Board< grid::Dimensions< 8,  8, true> >;

// the Frisian board needs at least 2 edge columns
// in order to accommodate orthogonal captures
using Frisian       = Board< grid::Dimensions<10, 10>, 2 >;

// rectangular boards always have opposite coloring,
// can have at most 1 edge column when larger than 10x10,
// and require internal rotations for 12x10 to fit within 64-bits
using Spantsireti   = Board< grid::Dimensions<10,  8, true> > ;
using Ktar11        = Board< grid::Dimensions<11, 10, true>, 1 >;
using Ktar12        = Board< grid::Dimensions<12, 10, true>, 1, angle::R090 >;

}       // namespace board
}       // namespace dctl
