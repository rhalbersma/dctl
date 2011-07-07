#include "Angle.h"

namespace board {

// partial specialization for identity rotations
template<int H, int W, bool P>
struct Rotate<Grid<H, W, P>, Angle::D000>
{
        typedef Grid<H, W, P> type;
};

// partial specialization for 90 degrees right rotations
template<int H, int W, bool P>
struct Rotate<Grid<H, W, P>, Angle::D270>
{
        typedef Grid<W, H, (W % 2) ^ !P> type;
};

// partial specialization for 90 degrees left rotations
template<int H, int W, bool P>
struct Rotate<Grid<H, W, P>, Angle::D090>
{
        typedef Grid<W, H, (H % 2) ^ !P> type;
};

// partial specialization for 180 degrees rotations
template<int H, int W, bool P>
struct Rotate<Grid<H, W, P>, Angle::D180>
{
        typedef Grid<H, W, (H % 2) ^ (W % 2) ^ static_cast<int>(P)> type;
};

}       // namespace board
