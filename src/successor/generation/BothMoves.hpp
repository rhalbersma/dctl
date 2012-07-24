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

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Moves, generation, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Moves, generation, Rules, Board> KingMoves;
        typedef Driver<Color, Material::pawn, select::Moves, generation, Rules, Board> PawnMoves;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                KingMoves::generate(p, moves);
                PawnMoves::generate(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
