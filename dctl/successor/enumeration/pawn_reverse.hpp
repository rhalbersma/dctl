#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/enumeration/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, Reverse, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef enumerator<!Color, Material::pawn, Moves, Position> PassivePawnMoves;

public:
        int operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        return PassivePawnMoves().select(active_pawns, not_occupied(p));
                else
                        return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
