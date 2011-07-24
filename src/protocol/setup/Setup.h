#pragma once

namespace dctl {
namespace protocol {
namespace setup {

template<char B, char W, char E>
struct Token
{
        static const char BLACK = B;
        static const char WHITE = W;
        static const char EMPTY = E;

        static const char COLOR[2];
        static const char LOWER[2];
        static const char UPPER[2];
};

template<typename> struct Setup;

}       // namespace setup
}       // namespace protocol
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Setup.hpp"
