#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // copy (king jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps
template<bool Color>
struct copy<Color, Material::king, select::jumps>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef impl::copy<Color, Material::king, select::jumps, Position> KingJumps;

                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
