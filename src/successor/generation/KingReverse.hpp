#pragma once
#include "Generator_fwd.hpp"
#include "KingMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::king, Reverse, Position>
:
        generator<Color, Material::king, Moves, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
