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
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                Propagate<Moves, Position> const propagate(p);
                aux::copy<Color, Material::king, select::moves, Position>{propagate, moves}(p.kings(Color));
                aux::copy<Color, Material::pawn, select::moves, Position>{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
