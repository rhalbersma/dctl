#pragma once
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/pawn_promotions.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Promotions, Position>
:
        generator<Color, Material::pawn, Promotions, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
