#pragma once
#include "../../Variants/Rules.h"

namespace tree {
namespace move {

template<char M, char C>
struct Doublet
{
        static const char MOVE    = M;
        static const char CAPTURE = C;
};

template<typename> struct Token		        { typedef Doublet<'-', 'x'> TYPE; };
template<> struct Token<variants::Russian>      { typedef Doublet<'-', ':'> TYPE; };
template<> struct Token<variants::Czech>	{ typedef Doublet<'-', ':'> TYPE; };
template<> struct Token<variants::Thai>         { typedef Doublet<'-', '-'> TYPE; };

}       // namespace move
}       // namespace move
