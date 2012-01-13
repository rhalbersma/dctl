#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Transform.hpp"
#include "../utility/Arithmetic.hpp"

namespace dctl {

/*
        Degrees are denoted as <D><XXX>, where XXX is exactly three characters 
        long, running from 000 until 360 in steps of 45. Counterclockwise 
        rotations are denoted as <L><XXX>, where <XXX> runs from 45 until 135.
        Clockwise rotations are similarly denoted as <R><XXX>. 

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
        BOOST_STATIC_CONSTANT(auto, L045 = D045);       // counterclockwise 
        BOOST_STATIC_CONSTANT(auto, L090 = D090);       // counterclockwise
        BOOST_STATIC_CONSTANT(auto, L135 = D135);       // counterclockwise
        BOOST_STATIC_CONSTANT(auto, R135 = D225);       // clockwise
        BOOST_STATIC_CONSTANT(auto, R090 = D270);       // clockwise
        BOOST_STATIC_CONSTANT(auto, R045 = D315);       // clockwise
};

template<int N>
struct mod_360
{
        BOOST_STATIC_CONSTANT(auto, value = (Modulo<N, Degrees::D360>::value));
};

template<int N>
struct mod_090
{
        BOOST_STATIC_CONSTANT(auto, value = (Modulo<N, Degrees::D090>::value));
};

// mirrored forward direction index (orthogonal to the original)
template<int N>
struct mirror_up
{
private:
        BOOST_STATIC_CONSTANT(auto, r = mod_360<N>::value);

public:
        BOOST_STATIC_CONSTANT(auto, value = 
                (r + Degrees::D090) % Degrees::D180 + 
                (r / Degrees::D180) * Degrees::D180
        );
};

// mirrored backward direction index (orthogonal to the original)
template<int N>
struct mirror_down
{
private:
        BOOST_STATIC_CONSTANT(auto, r = mod_360<N>::value);

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, L = (rotate<Angle<r>, Degrees::L090>::type::value));
        BOOST_STATIC_CONSTANT(auto, ML = mirror_up<L>::value);
        BOOST_STATIC_CONSTANT(auto, RML = (rotate<Angle<ML>, Degrees::R090>::type::value));

public:
        BOOST_STATIC_CONSTANT(auto, value = RML);
};

}       // namespace dctl
