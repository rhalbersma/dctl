#pragma once
#include "../Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Moves, detection, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::king, select::Moves, detection, Rules, Board> KingMoves;
        typedef Driver<Color, Material::pawn, select::Moves, detection, Rules, Board> PawnMoves;

public:
        template<typename Position>
        static bool detect(Position const& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnMoves::detect(p) ||
                        KingMoves::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
