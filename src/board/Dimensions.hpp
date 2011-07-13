#include "Angle.h"

namespace board {

// partial specialization for identity rotations
template<int H, int W, bool P>
struct Rotate<Dimensions<H, W, P>, Angle::D000>
{
        typedef Dimensions<H, W, P> type;
};

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct Rotate<Dimensions<H, W, P>, Angle::D270>
{
        typedef Dimensions<W, H, (W % 2) ^ !P> type;
};

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct Rotate<Dimensions<H, W, P>, Angle::D090>
{
        typedef Dimensions<W, H, (H % 2) ^ !P> type;
};

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct Rotate<Dimensions<H, W, P>, Angle::D180>
{
        typedef Dimensions<H, W, (H % 2) ^ (W % 2) ^ (!!P)> type;
};

}       // namespace board
