#pragma once
#include <dctl/angle/degrees.hpp>                       // D000, L090, R090, D180
#include <dctl/grid/dimensions.hpp>                    // Dimensions
#include <dctl/grid/dimensions/transform_fwd.hpp>      // rotate (partial specialization declarations)

namespace dctl {
namespace mpl {
namespace lazy {

// NOTE: because rotate is a LAZY metafunction, 
// its primary template definition needs to have been seen at this point

// partial specialization definitions

// partial specialization for identity rotations
template<int H, int W, bool P>
struct rotate< grid::Dimensions<H, W, P>, angle::Degrees<angle::D000> >
:
        grid::Dimensions<H, W, P>
{};

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct rotate< grid::Dimensions<H, W, P>, angle::Degrees<angle::L090> >
:
        grid::Dimensions<W, H, (H % 2) ^ (!P)>
{};

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct rotate< grid::Dimensions<H, W, P>, angle::Degrees<angle::R090> >
:
        grid::Dimensions<W, H, (W % 2) ^ (!P)>
{};

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct rotate< grid::Dimensions<H, W, P>, angle::Degrees<angle::D180> >
:
        grid::Dimensions<H, W, (H % 2) ^ (W % 2) ^ (!!P)>
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
