#pragma once
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/king_moves.hpp>
#include <dctl/successor/copy/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Moves, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                generator<Color, Material::king, Moves, Position>{moves}(p);
                generator<Color, Material::pawn, Moves, Position>()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
