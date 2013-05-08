#pragma once
#include <dctl/angle/degrees.hpp>       // D000, L090, R090, D180
#include <dctl/board/dimensions.hpp>    // Dimensions
#include <dctl/mpl/transform.hpp>       // rotate (primary template definition)

namespace dctl {
namespace mpl {
namespace lazy {

// partial specialization for identity rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::D000 >;

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::L090 >;

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::R090 >;

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct rotate< board::Dimensions<H, W, P>, angle::D180 >;

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
