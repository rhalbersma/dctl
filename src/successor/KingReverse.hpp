#pragma once
#include "Driver_fwd.hpp"
#include "KingMoves.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"

namespace dctl {
namespace successor {

// partial specialization for reverse king moves
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::king, select::Reverse, Rules, Board>
:
        Driver<Color, Material::king, select::Moves, Rules, Board>
{};

}       // namespace successor
}       // namespace dctl
