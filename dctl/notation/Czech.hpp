#pragma once
#include <dctl/notation/Notation_fwd.hpp>             // Format, Separator (primary templates)
#include <dctl/notation/Types.hpp>                    // Token
#include <dctl/rules/Czech_fwd.hpp>       // Czech

namespace dctl {
namespace notation {

template<>
struct Format<rules::Czech>
:
        algebraic
{};

}       // namespace notation
}       // namespace dctl
