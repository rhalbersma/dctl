#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {

struct Side
{
        BOOST_STATIC_CONSTANT(auto, black = false);
        BOOST_STATIC_CONSTANT(auto, white = true);
        BOOST_STATIC_CONSTANT(auto, red = black);
        BOOST_STATIC_CONSTANT(auto, pass = true);
};

}       // namespace dctl
