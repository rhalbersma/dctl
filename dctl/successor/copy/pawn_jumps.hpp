#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/aux/pawn_jumps.hpp>       // copy (pawn jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps
template<bool Color, typename Position>
struct copy<Color, Material::pawn, select::jumps, Position>
{
        template<typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef aux::copy<Color, Material::pawn, select::jumps, Position> PawnJumps;

                Propagate<select::jumps, Position> propagate(p);
                PawnJumps{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
