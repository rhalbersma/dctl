#pragma once
#include <functional>                   // function
#include "Generator_fwd.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Reverse, Position>
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef generator<!Color, Material::pawn, Moves, Position> PassivePawnMoves;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        PassivePawnMoves().select(active_pawns, not_occupied(p), moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
