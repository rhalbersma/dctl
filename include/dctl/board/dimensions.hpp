#pragma once
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/int.hpp>            // int_
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT

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
        using height = boost::mpl::int_<H>;
        using width = boost::mpl::int_<W>;
        using parity = boost::mpl::bool_<P>;

        // lazily evaluable metadata == nullary metafunction
        using type = Dimensions<H, W, P>;
};

}       // namespace board
}       // namespace dctl
