#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Degrees.hpp"                  // D000, L090, R090, D180
#include "Transform.hpp"                // rotate

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

        // lazily evaluable metadata == nullary metafunction
        typedef Dimensions<H, W, P> type;
};

}       // namespace board

// partial specialization for identity rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::D000 >
:
        board::Dimensions<H, W, P>
{};

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::L090 >
:
        board::Dimensions<W, H, (H % 2) ^ (!P)>
{};

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::R090 >
:
        board::Dimensions<W, H, (W % 2) ^ (!P)>
{};

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::D180 >
:
        board::Dimensions<H, W, (H % 2) ^ (W % 2) ^ (!!P)>
{};

}       // namespace dctl
