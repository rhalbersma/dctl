#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/pawn_promotions.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, Material::both, select::promotions, Range>
:
        detect<Color, Material::pawn, select::promotions, Range>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
