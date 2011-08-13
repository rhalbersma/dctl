#pragma once

namespace dctl {
namespace setup {

template<char B, char W, char E>
struct TokenSetBase
{
        static const char BLACK = B;
        static const char WHITE = W;
        static const char EMPTY = E;

        static const char COLOR[];
        static const char LOWER[];
        static const char UPPER[];
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
