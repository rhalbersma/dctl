#pragma once
#include <cstddef>

namespace Geometry {
namespace Squares {

template
<
        size_t H,       // height
        size_t W,       // width
        bool P = false  // start at top-left square?
>
class Grid
{
private:
        // range of even (E) and odd (O) rows
        static const size_t ROW_E = (W +  P) / 2;
        static const size_t ROW_O = (W + !P) / 2;

public:
        // reflection on template parameters
        static const size_t HEIGHT = H;
        static const size_t WIDTH = W;
        static const bool PARITY = P;

        // range of row pairs
        static const size_t MODULO = W;

        // left (L) and right (R) edges of even (E) and odd (O) rows
        static const size_t EDGE_LE = 0;
        static const size_t EDGE_RE = EDGE_LE + ROW_E - 1;
        static const size_t EDGE_LO = EDGE_RE + 1;
        static const size_t EDGE_RO = EDGE_LO + ROW_O - 1;

        // == (H * W) / 2 + (H * W * P) % 2
        static const size_t SIZE = MODULO * ((H - 1) / 2) + ((H % 2)? EDGE_RE : EDGE_RO) + 1;
};

template<typename, size_t> 
struct Rotate;

}       // namespace Squares
}       // namespace Geometry

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Squares.hpp"
