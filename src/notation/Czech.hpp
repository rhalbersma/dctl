#pragma once
#include "Notation_fwd.hpp"             // Format, Separator (primary templates)
#include "Types.hpp"                    // Token
#include "../rules/Czech_fwd.hpp"       // Czech

namespace dctl {
namespace notation {

template<>
struct Format<rules::Czech>
:
        algebraic
{};

}       // namespace notation
}       // namespace dctl
