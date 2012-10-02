#pragma once
#include <functional>                   // function
#include "Generator_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Moves, Position>
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Moves, Position> KingMoves;
        typedef generator<Color, Material::pawn, Moves, Position> PawnMoves;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (KingMoves(moves))(p);
                (PawnMoves(moves))(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
