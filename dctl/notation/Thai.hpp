#pragma once
#include <dctl/notation/Notation_fwd.hpp>             // Separator (primary template)
#include <dctl/notation/Types.hpp>                    // Token
#include <dctl/rules/Thai_fwd.hpp>        // Thai

namespace dctl {
namespace notation {

template<>
struct Separator<rules::Thai>
:
        detail::token<'-', '-'>
{};

}       // namespace notation
}       // namespace dctl
