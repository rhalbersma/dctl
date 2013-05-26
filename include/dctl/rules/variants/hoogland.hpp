#pragma once
#include <dctl/rules/variants/hoogland_fwd.hpp>
#include <dctl/rules/variants/international.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/hoogland.hpp>

namespace dctl {
namespace rules {

// H. Hoogland's proposal (1923)

struct Hoogland
:
        // main rules
        International
{
        // additional rules
        typedef orthogonality::relative orthogonality_king_jump;
};

}       // namespace rules
}       // namespace dctl
