#pragma once
#include "../Utilities/IntegerTypes.h"

template
<
        size_t N = 2,                           // number of neutral rows in the initial position
        size_t P = 1                            // number of promotion rows
>
struct Zones
{
        static const size_t NEUTRAL = N;
        static const size_t PROMOTE = P;
};
