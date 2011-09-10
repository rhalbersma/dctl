#pragma once
#include <boost/config.hpp>
#include "Transform.h"

namespace dctl {
namespace board {

template
<
        int H,          // height
        int W,          // width
        bool P = false  // parity (true for single corner at top-left square)
>
struct Dimensions
{
        // reflection on template parameters
        BOOST_STATIC_CONSTANT(auto, height = H);
        BOOST_STATIC_CONSTANT(auto, width = W);
        BOOST_STATIC_CONSTANT(auto, parity = P);
};

}       // namespace board

// partial specialization for dimensions
template<int H, int W, bool P, int Angle>
struct rotate<board::Dimensions<H, W, P>, Angle>;

}       // namespace dctl

// include template definitions inside header
#include "Dimensions.hpp"
