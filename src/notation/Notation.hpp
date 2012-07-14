#pragma once
#include "Types.hpp"                    // numeric, Token

namespace dctl {
namespace notation {

template<typename>
struct Format
{
        typedef numeric type;
};

template<typename>
struct Separator
:
        Token<'-', 'x'>
{};

}       // namespace notation
}       // namespace dctl
