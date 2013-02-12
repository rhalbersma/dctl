#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_moves.hpp>    // detect (king moves specialization)
#include <dctl/successor/detect/impl/pawn_moves.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, Material::king, select::reverse, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<!Color, Material::king, select::moves, Position, rules::range::distance_1> KingReverse;

                Propagate<select::moves, Position> const propagate(p);
                return KingReverse{propagate}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
