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

// partial specialization for dimensions
template<int H, int W, bool P, int I>
struct rotate<Dimensions<H, W, P>, I>;

}       // namespace board
}       // namespace dctl

// include template definitions inside header
#include "Dimensions.hpp"
