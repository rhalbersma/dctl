#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_moves.hpp>      // copy (king moves specialization)
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for reverse king moves
template<bool Color>
struct copy<Color, material::king, select::reverse>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef impl::copy<!Color, material::king, select::moves, Position> KingReverse;

                Propagate<select::moves, Position> const propagate(p);
                KingReverse{propagate, moves}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
