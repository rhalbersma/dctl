#pragma once
#include "../../Variants/Rules.h"

namespace Tree {
namespace Move {

template<char M, char C>
struct Doublet
{
        static const char MOVE    = M;
        static const char CAPTURE = C;
};

template<typename> struct Token		        { typedef Doublet<'-', 'x'> TYPE; };
template<> struct Token<Variants::Russian>      { typedef Doublet<'-', ':'> TYPE; };
template<> struct Token<Variants::Czech>	{ typedef Doublet<'-', ':'> TYPE; };

}       // namespace Move
}       // namespace Move
