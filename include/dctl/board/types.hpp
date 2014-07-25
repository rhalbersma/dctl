#pragma once
#include <dctl/board/board.hpp>         // Board

namespace dctl {
namespace board {

// smaller square boards (fit into 64-bits, even with orthogonal capture)
using Micro         = Board< 4,  4>;
using Mini          = Board< 6,  6>;
using Checkers      = Board< 8,  8>;
using International = Board<10, 10>;
using Frisian       = International;

// larger square boards (do not fit into 64-bits, even without orthogonal capture)
using Canadian      = Board<12, 12>;
using Dumm          = Board<14, 14>;

// lower left inverted square boards
using Roman         = Board< 8,  8, true>;
using SriLankan     = Board<12, 12, true>;

// rectangular boards with default inversion
using Spantsireti   = Board<10,  8>;

// rectangular boards with front lines of square boards of the same width
template<int Width, int Height>
using Ktar          = Board<Width, Height, true>;

// without orthogonal capture, Board<12, 10> and Ktar<10, 12> fit into 64-bits
using Compact1210   = Board<12, 10, false, false>;
using Compact1012   = Board<10, 12, true , false>;

}       // namespace board
}       // namespace dctl
