#pragma once
#include <dctl/successor/generation/Generator_fwd.hpp>
#include <dctl/successor/generation/PawnPromotions.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/node/Material.hpp>

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
