#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves detection
template<bool Color, typename Position, typename Range>
struct detect<Color, Material::king, select::moves, Position, Range>
{
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, Material::king, select::moves, Position, rules::range::distance_1> KingMoves;

                Propagate<select::moves, Position> const propagate(p);
                return KingMoves{propagate}(moveable_kings(p, Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
