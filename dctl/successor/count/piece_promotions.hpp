#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/pawn_promotions.hpp>     // count (pawn moves specialization)
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece promotions
template<bool Color>
struct count<Color, material::piece, select::promotions>
:
        count<Color, material::pawn, select::promotions>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
