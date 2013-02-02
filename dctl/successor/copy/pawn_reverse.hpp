#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/aux/pawn_moves.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/successor/select/reverse.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves
template<bool Color, typename Position>
struct copy<Color, Material::pawn, select::reverse, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef aux::copy<!Color, Material::pawn, select::moves, Position> PawnReverse;
                Propagate<select::moves, Position> propagate(p);
                PawnReverse{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
