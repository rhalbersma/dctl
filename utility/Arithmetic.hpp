#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT

namespace dctl {

template<int A, int N>
struct Modulo
{
        BOOST_STATIC_ASSERT(N > 0);
        BOOST_STATIC_CONSTANT(auto, value =                 
                (A % N + ((A < 0) ? N : 0))
        ); 
        BOOST_STATIC_ASSERT(0 <= value && value < N);
};

}       // namespace dctl
