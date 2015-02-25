#pragma once
#include <dctl/rules/hoogland_fwd.hpp>
#include <dctl/rules/international.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/capture/hoogland.hpp>

namespace dctl {
namespace rules {

// H. Hoogland's proposal (1923)

struct Hoogland
:
        // main rules
        International
{
        // additional rules
        typedef orthogonality::relative king_jump_orthogonality;
};

}       // namespace rules
}       // namespace dctl