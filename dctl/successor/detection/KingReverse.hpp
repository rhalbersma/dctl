#pragma once
#include <dctl/successor/detection/Detector_fwd.hpp>
#include <dctl/successor/detection/KingMoves.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/node/Material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct detector<Color, Material::king, Reverse, Position>
:
        detector<Color, Material::king, Moves, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
