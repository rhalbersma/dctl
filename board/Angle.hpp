#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Transform.hpp"

namespace dctl {

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

struct Degrees
{
        BOOST_STATIC_CONSTANT(auto, D000 =    0); 
        BOOST_STATIC_CONSTANT(auto, D045 =    1); 
        BOOST_STATIC_CONSTANT(auto, D090 =    2); 
        BOOST_STATIC_CONSTANT(auto, D135 =    3); 
        BOOST_STATIC_CONSTANT(auto, D180 =    4); 
        BOOST_STATIC_CONSTANT(auto, D225 =    5); 
        BOOST_STATIC_CONSTANT(auto, D270 =    6); 
        BOOST_STATIC_CONSTANT(auto, D315 =    7);
        BOOST_STATIC_CONSTANT(auto, D360 =    8); 
        BOOST_STATIC_CONSTANT(auto, L045 = D045);       // counter-clockwise 
        BOOST_STATIC_CONSTANT(auto, L090 = D090);       // counter-clockwise
        BOOST_STATIC_CONSTANT(auto, L135 = D135);       // counter-clockwise
        BOOST_STATIC_CONSTANT(auto, R135 = D225);       // clockwise
        BOOST_STATIC_CONSTANT(auto, R090 = D270);       // clockwise
        BOOST_STATIC_CONSTANT(auto, R045 = D315);       // clockwise
};

template<int Index>
struct Angle
{
        BOOST_STATIC_CONSTANT(auto, index = Index);
};

// partial specialization for angles
template<int Index, int Theta>
struct rotate<Angle<Index>, Theta>
{
        // angles are subject to arithmetic modulo 360 degrees, 
        // with the unit element equal to 45 degrees anti-clockwise
        typedef Angle<(Index + Theta) % Degrees::D360> type;
};

// an inverse angle has the opposite sign modulo 360 degrees
template<int Index>
struct inverse
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Degrees::D360 - Index) % Degrees::D360
        );
};

// mirrored forward direction index (orthogonal to the original)
template<int Index>
struct mirror_up
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Index + Degrees::D090) % Degrees::D180 + (Index / Degrees::D180) * Degrees::D180
        );
};

// mirrored backward direction index (orthogonal to the original)
template<int I>
struct mirror_down
{
private:
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, L = (rotate<Angle<I>, Degrees::L090>::type::index));
        BOOST_STATIC_CONSTANT(auto, ML = mirror_up<L>::value);
        BOOST_STATIC_CONSTANT(auto, RML = (rotate<Angle<ML>, Degrees::R090>::type::index));

public:
        // M' == R M L
        BOOST_STATIC_CONSTANT(auto, value = RML);
};

}       // namespace dctl
