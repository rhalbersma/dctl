#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace dxp {

// type traits
template<typename Protocol> 
struct port                  
{ 
        BOOST_STATIC_CONSTANT(unsigned short, value = 27531);
};

template<typename Protocol> 
struct terminator            
{ 
        BOOST_STATIC_CONSTANT(auto, value = '\0'); 
};

template<typename Protocol>
struct header_length
{ 
        BOOST_STATIC_CONSTANT(auto, value = 1); 
};

template<typename Protocol> 
struct max_body_length
{ 
        BOOST_STATIC_CONSTANT(auto, value = 126);
};

}       // namespace dxp
}       // namespace dctl
