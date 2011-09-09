#pragma once
#include <boost/config.hpp>

namespace dctl {
namespace setup {

template<char B, char W, char E>
struct TokenSetBase
{
        BOOST_STATIC_CONSTANT(auto, BLACK = B);
        BOOST_STATIC_CONSTANT(auto, WHITE = W);
        BOOST_STATIC_CONSTANT(auto, EMPTY = E);

        static const char color[];
        static const char lower[];
        static const char upper[];
};

template<typename> struct TokenSet;

template<typename Protocol>
struct Token
{
        typedef TokenSet<Protocol> type;
};

}       // namespace setup
}       // namespace dctl

// include template definitions inside header
#include "Token.hpp"
