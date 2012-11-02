#pragma once
#include <dctl/notation/notation_fwd.hpp>       // Format, Separator (primary templates)
#include <dctl/notation/types.hpp>              // Token
#include <dctl/rules/czech_fwd.hpp>             // Czech

namespace dctl {
namespace notation {

template<>
struct Format<rules::Czech>
:
        algebraic
{};

}       // namespace notation
}       // namespace dctl
