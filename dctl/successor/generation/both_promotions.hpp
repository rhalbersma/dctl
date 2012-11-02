#pragma once
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/pawn_promotions.hpp>
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
