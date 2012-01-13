#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Transform.hpp"

namespace dctl {
namespace board {

template
<
        int H,          // height
        int W,          // width
        bool P = false  // parity (== single corner at top-left square)
>
struct Dimensions
{
        BOOST_STATIC_ASSERT(H > 0 && W > 0);

        // reflection on template parameters
        BOOST_STATIC_CONSTANT(auto, height = H);
        BOOST_STATIC_CONSTANT(auto, width = W);
        BOOST_STATIC_CONSTANT(auto, parity = P);
};

}       // namespace board

// reduce to a template specialiation for 0 <= N < 360 degrees
template<int H, int W, bool P, int N>
struct rotate<board::Dimensions<H, W, P>, N>
{
        // rotations of Dimensions have to be a multiple of 90 degrees
        BOOST_STATIC_ASSERT(!mod_090<N>::value);
        typedef typename rotate<
                board::Dimensions<H, W, P>, 
                mod_360<N>::value
        >::type type;
};

// partial specialization for identity rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::D000 >
{
        typedef board::Dimensions<H, W, P> type;
};

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::L090 >
{
        typedef board::Dimensions<W, H, (H % 2) ^ (!P)> type;
};

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::R090 >
{
        typedef board::Dimensions<W, H, (W % 2) ^ (!P)> type;
};

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct rotate<board::Dimensions<H, W, P>, Degrees::D180 >
{
        typedef board::Dimensions<H, W, (H % 2) ^ (W % 2) ^ (!!P)> type;
};

}       // namespace dctl
