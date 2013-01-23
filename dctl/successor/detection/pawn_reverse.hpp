#pragma once
#include <dctl/successor/detection/detector_fwd.hpp>
#include <dctl/successor/detection/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct detector<Color, Material::pawn, Reverse, Position>
{
private:
        // typedefs

        typedef detector<!Color, Material::pawn, Moves, Position> PassivePawnMoves;

public:
        bool operator()(Position const& p) const
        {
                auto const active_pawns = p.pawns(Color);
                return active_pawns? PassivePawnMoves().select(active_pawns, not_occupied(p)) : false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
