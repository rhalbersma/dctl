#pragma once
#include <dctl/successor/detection/detector_fwd.hpp>
#include <dctl/successor/detection/king_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>

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
