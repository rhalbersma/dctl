#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Degrees.hpp"
#include "Transform.hpp"

namespace dctl {

template<int N>
struct Angle
{
        // angles are subject to arithmetic modulo 360 degrees
        BOOST_STATIC_CONSTANT(auto, value = mod_360<N>::value);
};

template<int N>
struct inverse
{
        // an inverse angle has the opposite sign modulo 360 degrees
        typedef Angle<mod_360<-N>::value> type;
};

// partial specialization for angles
template<int M, int N>
struct rotate<Angle<M>, N>
{
        // angles are subject to arithmetic modulo 360 degrees
        typedef Angle<mod_360<M + N>::value> type;
};

}       // namespace dctl
