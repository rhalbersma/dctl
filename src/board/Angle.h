#pragma once
#include <boost/config.hpp>
#include "Transform.h"
#include "../utils/TemplateTricks.h"

namespace dctl {
namespace board {

// angles are subject to arithmetic modulo 360 degrees, 
// with the unit element equal to 45 degrees anti-clockwise
/*
                D090 = L090
                     |
        D135 = L135  |  D045 = L045
                   \ | / 
                    \|/
           D180 ----- ----- D000 = D360
                    /|\
                   / | \
        D225 = R135  |  D315 = R045
                     |
                D270 = R090
*/

struct Angle
{
        enum {
                D000 = 0, D045 =    1, D090 =    2, D135 =    3, 
                D180 = 4, D225 =    5, D270 =    6, D315 =    7,
                D360 = 8, L045 = D045, L090 = D090, L135 = D135, 
                          R135 = D225, R090 = D270, R045 = D315 
        };
};

// partial specialization for angles
template<int A, int I>
struct rotate<Int2Type<A>, I>;

}       // namespace board
}       // namespace dctl

// include template definitions inside header
#include "Angle.hpp"
