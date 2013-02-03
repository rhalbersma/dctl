#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/pawn_promotions.hpp>      // copy (pawn promotions specialization)
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

template<bool Color>
struct copy<Color, Material::both, select::promotions>
:
        copy<Color, Material::pawn, select::promotions>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
