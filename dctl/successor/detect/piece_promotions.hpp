#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/pawn_promotions.hpp>
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/select/promotions.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, material::piece, select::promotions, Range>
:
        detect<Color, material::pawn, select::promotions, Range>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
