#pragma once
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/detect/pawn_promotions.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct detector<Color, Material::both, select::promotions, Position>
:
        detector<Color, Material::pawn, select::promotions, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
