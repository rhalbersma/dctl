#include "Angle.h"

namespace dctl {

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
