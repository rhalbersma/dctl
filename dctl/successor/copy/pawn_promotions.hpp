#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/aux/pawn_moves.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves
template<bool Color, typename Position>
struct copy<Color, Material::pawn, select::promotions, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef aux::copy<Color, Material::pawn, select::moves, Position> PawnMoves;
                Propagate<select::moves, Position> propagate(p);
                PawnMoves{propagate, moves}(promoting_pawns<Color>(p));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
