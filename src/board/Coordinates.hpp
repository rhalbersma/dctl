#include "Angle.h"

namespace board {

// partial specialization for identity rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D000>
{
        typedef Coordinates<G, R, C> type;
};

// partial specialization for 90 degrees right rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D270>
{
        typedef Coordinates<G, (G::WIDTH - 1) - C, R> type;
};

// partial specialization for 90 degrees left rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D090>
{
        typedef Coordinates<G, C, (G::HEIGHT - 1) - R> type;
};

// partial specialization for 180 degrees rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D180>
{
        typedef Coordinates<G, (G::HEIGHT - 1) - R, (G::WIDTH - 1) - C> type;
};

}       // namespace board
