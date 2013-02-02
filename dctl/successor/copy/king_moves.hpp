#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/aux/king_moves.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves
template<bool Color, typename Position>
struct copy<Color, Material::king, select::moves, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef aux::copy<Color, Material::king, select::moves, Position> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                PawnMoves{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
