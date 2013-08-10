#pragma once
#include <dctl/notation/types.hpp>      // numeric, Token

namespace dctl {
namespace notation {

template<class>
struct Format
{
        using type = numeric;
};

template<class>
struct Separator
:
        detail::token<'-', 'x'>
{};

}       // namespace notation
}       // namespace dctl
