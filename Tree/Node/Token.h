#pragma once
#include "Protocol.h"

namespace Tree {
namespace Node {

template<char B, char W, char E>
struct Triplet
{
        static const char BLACK = B;
        static const char WHITE = W;
        static const char EMPTY = E;

        static const char COLOR[2];
        static const char LOWER[2];
        static const char UPPER[2];
};

template<typename> struct Token;

template<>
struct Token<FEN_tag>: public Triplet<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '\"';
};

template<>
struct Token<DXP_tag>: public Triplet<'Z', 'W', 'e'> {};

}       // namespace Node
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Token.hpp"
