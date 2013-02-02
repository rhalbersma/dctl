#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/aux/king_moves.hpp>
#include <dctl/successor/copy/aux/pawn_moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct copy<Color, Material::both, select::moves, Position>
{
private:
        // typedefs

        typedef aux::copy<Color, Material::king, select::moves, Position> KingMoves;
        typedef aux::copy<Color, Material::pawn, select::moves, Position> PawnMoves;

public:
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                Propagate<select::moves, Position> const propagate(p);
                KingMoves{propagate, moves}(p.kings(Color));
                PawnMoves{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
