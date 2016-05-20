#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular

namespace dctl {
namespace board {

// smaller square boards (fit into 64-bits, even with orthogonal capture)
struct micro         : rectangular< 4,  4> {};
struct mini          : rectangular< 6,  6> {};
struct checkers      : rectangular< 8,  8> {};
struct roman         : rectangular< 8,  8, true> {};
struct international : rectangular<10, 10> {};
struct frisian       : rectangular<10, 10, false, true> {};

struct spantsireti   : rectangular<10,  8> {};

// rectangular boards with front line layouts of square boards of the same width
template<std::size_t Width, std::size_t Height, bool IsOrthogonalCaptures = true>
struct ktar          : rectangular<Width, Height, true, IsOrthogonalCaptures> {};

// without orthogonal captures, ktar<10, 12> and rectangular<12, 10>fit into 64-bits
struct compact_10_12 : ktar<10, 12, false> {};
struct compact_12_10 : rectangular<12, 10, false, false> {};

// larger square boards (do not fit into 64-bits, even without orthogonal capture)
struct canadian      : rectangular<12, 12> {};
struct srilankan     : rectangular<12, 12, true> {};
struct dumm          : rectangular<14, 14> {};

}       // namespace board
}       // namespace dctl
