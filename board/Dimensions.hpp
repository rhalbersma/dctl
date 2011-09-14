#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Angle.hpp"
#include "Transform.hpp"

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

// partial specialization for identity rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::D000>
{
        typedef board::Dimensions<H, W, P> type;
};

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::D270>
{
        typedef board::Dimensions<W, H, (W % 2) ^ !P> type;
};

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::D090>
{
        typedef board::Dimensions<W, H, (H % 2) ^ !P> type;
};

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::D180>
{
        typedef board::Dimensions<H, W, (H % 2) ^ (W % 2) ^ (!!P)> type;
};

}       // namespace dctl
