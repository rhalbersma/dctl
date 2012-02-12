#pragma once
#include "KingMoves.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"

namespace dctl {
namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> struct Driver;

// partial specialization for reverse king moves
template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::king, Reverse, Rules, Board>
:
        Driver<Color, Material::king, Moves, Rules, Board>
{};

}       // namespace successor
}       // namespace dctl
