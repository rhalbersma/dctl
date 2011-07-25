#pragma once

namespace dctl {
namespace setup {

template<char B, char W, char E>
struct Token
{
        static const char BLACK = B;
        static const char WHITE = W;
        static const char EMPTY = E;

        static const char COLOR[];
        static const char LOWER[];
        static const char UPPER[];
};

template<typename> struct Setup;

}       // namespace setup
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Setup.hpp"
