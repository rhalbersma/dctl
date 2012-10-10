#pragma once
#include "Generator_fwd.hpp"
#include "PawnPromotions.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"

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
