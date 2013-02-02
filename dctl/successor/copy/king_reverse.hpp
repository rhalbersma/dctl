#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/king_moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/successor/select/reverse.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct copy<Color, Material::king, select::reverse, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef aux::copy<!Color, Material::king, select::moves, Position> KingReverse;
                Propagate<select::moves, Position> propagate(p);
                KingReverse{propagate, moves}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
