#pragma once
#include <dctl/notation/types.hpp>      // numeric, Token

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
        detail::token<'-', 'x'>
{};

}       // namespace notation
}       // namespace dctl
