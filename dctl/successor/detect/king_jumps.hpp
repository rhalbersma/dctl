#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_jumps.hpp>    // detect (king jumps specialization)
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps detection
template<bool Color, typename Range>
struct detect<Color, material::king, select::jumps, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, material::king, select::jumps, Position, Range> KingJumps;

                Propagate<select::jumps, Position> const propagate(p);
                return KingJumps{propagate}(p.material().kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
