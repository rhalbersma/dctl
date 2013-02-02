#pragma once
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/detect/king_moves.hpp>
#include <dctl/successor/select/reverse.hpp>
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
