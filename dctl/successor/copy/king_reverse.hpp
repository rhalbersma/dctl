#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/king_moves.hpp>           // copy (king moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct copy<Color, Material::king, select::reverse, Position>
{
        template<typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef aux::copy<!Color, Material::king, select::moves, Position> KingReverse;

                Propagate<select::moves, Position> propagate(p);
                KingReverse{propagate, moves}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
