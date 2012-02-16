#pragma once
#include "Driver_fwd.hpp"
#include "PawnPromotion.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::both, select::Promotions, Rules, Board>
:
        Driver<Color, Material::pawn, select::Promotions, Rules, Board>
{};

}       // namespace successor
}       // namespace dctl
