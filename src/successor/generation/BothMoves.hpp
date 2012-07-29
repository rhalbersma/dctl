#pragma once
#include "../Driver_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "../Result.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Position>
struct generator<Color, Material::both, select::Moves, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs
        typedef generator<Color, Material::king, select::Moves, Position> KingMoves;
        typedef generator<Color, Material::pawn, select::Moves, Position> PawnMoves;

public:
        static void run(Position const& p, Stack& moves)
        {
                KingMoves::run(p, moves);
                PawnMoves::run(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
