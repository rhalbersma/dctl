#pragma once
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/int.hpp>            // int_
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include <dctl/angle/Degrees.hpp>       // D000, L090, R090, D180
#include <dctl/mpl/transform.hpp>       // rotate

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
        typedef boost::mpl::int_<H> height;
        typedef boost::mpl::int_<W> width;
        typedef boost::mpl::bool_<P> parity;

        // lazily evaluable metadata == nullary metafunction
        typedef Dimensions<H, W, P> type;
};

}       // namespace board

namespace mpl {

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

}	// namespace mpl
}       // namespace dctl
