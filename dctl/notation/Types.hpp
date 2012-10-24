#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace notation {
namespace detail {

template<char M, char J>
struct token
{
        BOOST_STATIC_CONSTANT(auto, move = M);
        BOOST_STATIC_CONSTANT(auto, jump = J);
};

}

struct numeric {};              // numeric
typedef numeric algebraic;      // algebraic

}       // namespace notation
}       // namespace dctl
