#pragma once
#include "../../rules/Variants.h"

namespace tree {
namespace move {

template<char M, char C>
struct Doublet
{
        static const char MOVE    = M;
        static const char CAPTURE = C;
};

template<typename> struct Token		{ typedef Doublet<'-', 'x'> type; };
template<> struct Token<rules::Russian> { typedef Doublet<'-', ':'> type; };
template<> struct Token<rules::Czech>	{ typedef Doublet<'-', ':'> type; };
template<> struct Token<rules::Thai>    { typedef Doublet<'-', '-'> type; };

}       // namespace move
}       // namespace move
