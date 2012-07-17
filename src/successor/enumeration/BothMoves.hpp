#pragma once
#include "../Driver_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../../node/Material.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Moves, enumeration, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Moves, enumeration, Rules, Board> KingMoves;
        typedef Driver<Color, Material::pawn, select::Moves, enumeration, Rules, Board> PawnMoves;

public:
        template<typename Position>
        static int count(Position const& p)
        {
                return (
                        KingMoves::count(p) +
                        PawnMoves::count(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
