#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/aux/king_jumps.hpp>
#include <dctl/successor/propagate/jumps.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps
template<bool Color, typename Position>
struct copy<Color, Material::king, select::jumps, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef aux::copy<Color, Material::king, select::jumps, Position> KingJumps;
                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
