#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/pawn_promotions.hpp>     // count (pawn moves specialization)
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for combined king and pawn promotions
template<bool Color>
struct count<Color, Material::both, select::promotions>
:
        count<Color, Material::pawn, select::promotions>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
