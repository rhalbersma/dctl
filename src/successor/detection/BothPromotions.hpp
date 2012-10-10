#pragma once
#include "Detector_fwd.hpp"
#include "PawnPromotions.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct detector<Color, Material::both, Promotions, Position>
:
        detector<Color, Material::pawn, Promotions, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
