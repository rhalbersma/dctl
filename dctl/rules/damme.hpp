#pragma once
#include <dctl/rules/damme_fwd.hpp>
#include <dctl/rules/international.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/capture/damme.hpp>

namespace dctl {
namespace rules {

// A.K.W. Damme's proposal (1922)

struct Damme
:
        // main rules
        International
{
        // additional rules
        typedef range::distance_1 halt_range;
};

}       // namespace rules
}       // namespace dctl
