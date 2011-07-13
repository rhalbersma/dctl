#pragma once
#include "Angle.h"

namespace board {

template
<
        int D = 2,              // "demilitarized" rows in the initial position
        int G = 2,              // number of ghost bit columns
        int A = Angle::D000     // rotation from external to internal grid
>
struct Structure
{
        // reflection on template parameters
        static const int DMZ = D;
        static const int GHOSTS = G;
        static const int ANGLE = A;
        static const int A_INV = Inverse<A>::value;
};

}       // namespace board
