#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/king_moves.hpp>
#include <dctl/successor/select/reverse.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct detect<Color, Material::king, select::reverse, Position>
:
        detect<Color, Material::king, select::moves, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
