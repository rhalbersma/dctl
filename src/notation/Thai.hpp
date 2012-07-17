#pragma once
#include "Notation_fwd.hpp"             // Separator (primary template)
#include "Types.hpp"                    // Token
#include "../rules/Thai_fwd.hpp"        // Thai

namespace dctl {
namespace notation {

template<> 
struct Separator<rules::Thai>
:
        detail::token<'-', '-'>
{};

}       // namespace notation
}       // namespace dctl
