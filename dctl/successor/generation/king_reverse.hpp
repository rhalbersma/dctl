#pragma once
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/king_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>

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
