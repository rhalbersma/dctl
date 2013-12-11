#pragma once
#include <dctl/notation/notation_fwd.hpp>       // Format, Separator (primary templates)
#include <dctl/notation/types.hpp>              // Token
#include <dctl/rules/variants/russian_fwd.hpp>           // Russian

namespace dctl {
namespace notation {
/*
template<>
struct Format<rules::Russian>
:
        algebraic
{};
*/
template<>
struct Separator<rules::Russian>
:
        detail::token<'-', ':'>
{};

}       // namespace notation
}       // namespace dctl
