#pragma once
#include <dctl/rules/lineo_fwd.hpp>
#include <dctl/rules/international.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/lineo.hpp>

namespace dctl {
namespace rules {

// http://laatste.info/bb3/viewtopic.php?t=2598
// P. Schuitema's proposal (2009)

struct Lineo
:
        // main rules
        International
{
        // additional rules
        typedef orthogonality::relative king_move_orthogonality;
};

}       // namespace rules
}       // namespace dctl
