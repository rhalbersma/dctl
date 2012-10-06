#pragma once
#include <functional>                   // function
#include "Detector_fwd.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../node/UnaryProjections.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct detector<Color, Material::pawn, Reverse, Position>
:
        public std::function<bool(Position const&)>
{
private:
        // typedefs

        typedef detector<!Color, Material::pawn, Moves, Position> PassivePawnMoves;

public:
        bool operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        return PassivePawnMoves::select(active_pawns, not_occupied(p));
                else
                        return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
