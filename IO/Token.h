#pragma once

template<char B, char W, char E>
struct PositionTokenTriple
{
        static const char BLACK = B;
        static const char WHITE = W;
        static const char EMPTY = E;

        static const char COLOR[2];
        static const char LOWER[2];
        static const char UPPER[2];
};

template<char M, char C>
struct MoveTokenDouble
{
        static const char MOVE    = M;
        static const char CAPTURE = C;
};

static const char WHITE_SPACE = ' ';

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Token.hpp"
