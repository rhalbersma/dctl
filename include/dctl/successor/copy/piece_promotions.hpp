#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/pawn_promotions.hpp>      // copy (pawn promotions specialization)
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

template<bool Color>
struct copy<Color, material::piece, select::promotions>
:
        copy<Color, material::pawn, select::promotions>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
