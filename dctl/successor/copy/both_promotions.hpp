#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/pawn_promotions.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct copy<Color, Material::both, select::promotions, Position>
:
        copy<Color, Material::pawn, select::promotions, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
