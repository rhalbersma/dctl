#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // copy (king jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/king.hpp>             // king

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps
template<bool Color>
struct copy<Color, pieces::king, select::jumps>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                typedef impl::copy<Color, pieces::king, select::jumps, Position, Sequence> KingJumps;

                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(p.material().kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
