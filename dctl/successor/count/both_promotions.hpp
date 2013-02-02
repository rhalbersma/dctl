#pragma once
#include <dctl/successor/count/enumerator_fwd.hpp>
#include <dctl/successor/count/pawn_promotions.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, select::promotions, Position>
:
        enumerator<Color, Material::pawn, select::promotions, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
