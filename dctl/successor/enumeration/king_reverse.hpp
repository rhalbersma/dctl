#pragma once
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/enumeration/king_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>

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
