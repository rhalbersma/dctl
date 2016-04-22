#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular

namespace dctl {
namespace board {

// smaller square boards (fit into 64-bits, even with orthogonal capture)
using micro         = rectangular< 4,  4>;
using mini          = rectangular< 6,  6>;
using checkers      = rectangular< 8,  8>;
using roman         = rectangular< 8,  8, true>;
using spantsireti   = rectangular<10,  8>;
using international = rectangular<10, 10>;
using frisian       = rectangular<10, 10, false, true>;

// rectangular boards with front lines of square boards of the same width
template<int Width, int Height>
using ktar          = rectangular<Width, Height, true>;

// without orthogonal capture, rectangular<12, 10> and ktar<10, 12> fit into 64-bits
using compact1012   = rectangular<10, 12, true , false>;
using compact1210   = rectangular<12, 10, false, false>;

// larger square boards (do not fit into 64-bits, even without orthogonal capture)
using canadian      = rectangular<12, 12>;
using srilankan     = rectangular<12, 12, true>;
using dumm          = rectangular<14, 14>;

}       // namespace board
}       // namespace dctl
