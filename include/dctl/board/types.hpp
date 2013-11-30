#pragma once
#include <dctl/angle.hpp>               // _deg
#include <dctl/board/board.hpp>         // Board
#include <dctl/grid/dimensions.hpp>     // Dimensions

namespace dctl {
namespace board {

// square boards
using Micro         = Board< grid::Dimensions< 4,  4> >;
using Mini          = Board< grid::Dimensions< 6,  6> >;
using Checkers      = Board< grid::Dimensions< 8,  8> >;
using International = Board< grid::Dimensions<10, 10> >;
using Canadian      = Board< grid::Dimensions<12, 12> >;

// the Spanish-Italian board has opposite coloring
using Roman         = Board< grid::Dimensions< 8,  8, true> >;

// the Frisian board needs to explicitly accommodate orthogonal captures
using Frisian       = Board< grid::Dimensions<10, 10>, true >;

// rectangular boards always have opposite coloring,
// can have at most 1 edge column when larger than 10x10,
// and require internal rotations for 12x10 to fit within 64-bits
using Spantsireti   = Board< grid::Dimensions<10,  8, true> >;
using Ktar11        = Board< grid::Dimensions<11, 10, true> >;          // fits within 64-bits
using Ktar12        = Board< grid::Dimensions<12, 10, true>, false >;   // fits within 64-bits

}       // namespace board
}       // namespace dctl
