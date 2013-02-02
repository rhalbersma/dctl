#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/count/king_moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/successor/select/reverse.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct count<Color, Material::king, select::reverse, Position>
:
        count<Color, Material::king, select::moves, Position>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
