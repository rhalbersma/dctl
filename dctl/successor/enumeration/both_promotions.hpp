#pragma once
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/enumeration/pawn_promotions.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, Promotions, Position>
:
        enumerator<Color, Material::pawn, Promotions, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
