#pragma once
#include "Enumerator_fwd.hpp"
#include "PawnPromotions.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"

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
