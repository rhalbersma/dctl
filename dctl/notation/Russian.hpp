#pragma once
#include <dctl/notation/Notation_fwd.hpp>             // Format, Separator (primary templates)
#include <dctl/notation/Types.hpp>                    // Token
#include <dctl/rules/Russian_fwd.hpp>     // Russian

namespace dctl {
namespace notation {

template<>
struct Format<rules::Russian>
:
        algebraic
{};

template<>
struct Separator<rules::Russian>
:
        detail::token<'-', ':'>
{};

}       // namespace notation
}       // namespace dctl
