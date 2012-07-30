#pragma once
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
struct generator<Color, Material::both, select::Moves, Position>
{
private:
        // typedefs
        typedef generator<Color, Material::king, select::Moves, Position> KingMoves;
        typedef generator<Color, Material::pawn, select::Moves, Position> PawnMoves;

public:
        void operator()(Position const& p, Stack& moves)
        {
                KingMoves()(p, moves);
                PawnMoves()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
