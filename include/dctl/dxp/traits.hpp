#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace dxp {

// type traits
template<class>
struct port
{
        BOOST_STATIC_CONSTANT(unsigned short, value = 27531);
};

template<class>
struct terminator
{
        BOOST_STATIC_CONSTANT(auto, value = '\0');
};

}       // namespace dxp
}       // namespace dctl
