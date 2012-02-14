#pragma once
#include "Driver_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "Selection_fwd.hpp"
#include "../node/Material.hpp"
#include "../node/Position_fwd.hpp"
#include "../node/Stack_fwd.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::both, select::Moves, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Moves, Rules, Board> KingMoves;
        typedef Driver<Color, Material::pawn, select::Moves, Rules, Board> PawnMoves;

public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                KingMoves::generate(p, moves);
                PawnMoves::generate(p, moves);
        }

        static int count(const Position<Board>& p)
        {     
                return (
                        KingMoves::count(p) + 
                        PawnMoves::count(p)
                );
        }

        static bool detect(const Position<Board>& p)
        {
                return (
                        PawnMoves::detect(p) || 
                        KingMoves::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
