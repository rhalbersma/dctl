#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/king_moves.hpp>     // count (king moves specialization)
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for reverse king moves
template<bool Color>
struct count<Color, material::king, select::reverse>
{
        template<typename Position>
        int operator()(Position const& p) const
        {
                typedef impl::count<!Color, material::king, select::moves, Position> KingReverse;

                Propagate<select::moves, Position> const propagate(p);
                return KingReverse{propagate}(p.material().kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
