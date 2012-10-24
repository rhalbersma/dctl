#pragma once
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/enumeration/KingMoves.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/node/Material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::king, Reverse, Position>
:
        enumerator<Color, Material::king, Moves, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
