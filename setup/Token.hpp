#pragma once
#include <cctype>                       // toupper, tolower
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace setup {

template<typename> 
struct TokenSet;

template<typename Protocol>
struct Token
{
        typedef TokenSet<Protocol> type;
};

template<char B, char W, char E>
struct TokenSetBase
{
        BOOST_STATIC_CONSTANT(auto, black = B);
        BOOST_STATIC_CONSTANT(auto, white = W);
        BOOST_STATIC_CONSTANT(auto, empty = E);

        static const char color[];
        static const char lower[];
        static const char upper[];
};

template<char B, char W, char E>
const char TokenSetBase<B, W, E>::color[] = {
        black,
        white
};

template<char B, char W, char E>
const char TokenSetBase<B, W, E>::lower[] = {
        static_cast<char>(tolower(black)),
        static_cast<char>(tolower(white))
};

template<char B, char W, char E>
const char TokenSetBase<B, W, E>::upper[] = {
        static_cast<char>(toupper(black)),
        static_cast<char>(toupper(white))
};

}       // namespace setup
}       // namespace dctl
