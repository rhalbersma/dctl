#pragma once

namespace board {
namespace grid {

template
<
        int H,          // height
        int W,          // width
        bool P = false  // start at top-left square?
>
class Grid
{
private:
        // range of even (E) and odd (O) rows
        static const int ROW_E = (W +  P) / 2;
        static const int ROW_O = (W + !P) / 2;

public:
        // reflection on template parameters
        static const int HEIGHT = H;
        static const int WIDTH = W;
        static const bool PARITY = P;

        // range of row pairs
        static const int MODULO = W;

        // left (L) and right (R) edges of even (E) and odd (O) rows
        static const int EDGE_LE = 0;
        static const int EDGE_RE = EDGE_LE + ROW_E - 1;
        static const int EDGE_LO = EDGE_RE + 1;
        static const int EDGE_RO = EDGE_LO + ROW_O - 1;

        // == (H * W) / 2 + (H * W * P) % 2
        static const int SIZE = MODULO * ((H - 1) / 2) + ((H % 2)? EDGE_RE : EDGE_RO) + 1;
};

template<typename, int> 
struct Rotate;

}       // namespace grid
}       // namespace board

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Grid.hpp"
