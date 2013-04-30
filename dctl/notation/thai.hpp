#pragma once
#include <dctl/notation/notation_fwd.hpp>       // Separator (primary template)
#include <dctl/notation/types.hpp>              // Token
#include <dctl/rules/variants/thai_fwd.hpp>              // Thai

namespace dctl {
namespace notation {

template<>
struct Separator<rules::Thai>
:
        detail::token<'-', '-'>
{};

}       // namespace notation
}       // namespace dctl
