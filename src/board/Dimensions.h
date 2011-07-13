#pragma once

namespace board {

template
<
        int H,          // height
        int W,          // width
        bool P = false  // parity (== start at top-left square)
>
struct Dimensions
{
        // reflection on template parameters
        static const int HEIGHT = H;
        static const int WIDTH = W;
        static const bool PARITY = P;
};

}       // namespace board

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Dimensions.hpp"
