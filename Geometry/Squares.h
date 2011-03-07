#pragma once
#include <cstddef>

namespace Geometry {
namespace Squares {

template
<
        size_t H,                                               // height
        size_t W,                                               // width
        bool P = false                                          // coloring of top-left square
>
struct Grid
{
        // reflection on template parameters
        static const size_t HEIGHT = H;
        static const size_t WIDTH = W;
        static const bool PARITY = P;

        // number of squares per row and row pair
        static const size_t ROW_E = (W +  P) / 2;               // range of even rows
        static const size_t ROW_O = (W + !P) / 2;               // range of odd rows
        static const size_t MODULO = W;                         // range of row pairs

        // boundaries of even and odd rows
        static const size_t EDGE_LE = 0;                        // left edge of even rows
        static const size_t EDGE_RE = EDGE_LE + ROW_E - 1;  	// right edge of even rows
        static const size_t EDGE_LO = EDGE_RE + 1;              // left edge of odd rows
        static const size_t EDGE_RO = EDGE_LO + ROW_O - 1;      // right edge of even rows

        // == (H * W) / 2 + (H * W * P) % 2
        static const size_t SIZE = MODULO * ((H - 1) / 2) + ((H % 2)? EDGE_RE : EDGE_RO) + 1;
};

template<typename, size_t> 
struct Rotate;

}       // namespace Squares
}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Squares.hpp"
