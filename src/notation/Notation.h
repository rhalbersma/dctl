#pragma once
#include "Types.h"
#include "Variants.h"

namespace dctl {
namespace notation {

template<typename> struct Format        { typedef numeric type; };

template<char M, char J>
struct Token
{
        static const char move = M;
        static const char jump = J;
};

template<typename> struct Separator     { typedef Token<'-', 'x'> type; };

}       // namespace notation
}       // namespace dctl
