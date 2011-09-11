#pragma once
#include "../node/Material.h"
#include "../node/Stack.h"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

namespace successor {

template<bool Color, typename Selection, typename Rules, typename Board> 
 
class Driver<Color, Material::BOTH, Selection, Rules, Board>
:
        private utility::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // typedefs
        typedef Driver<Color, Material::KING, Moves, Rules, Board> KingMoves;
        typedef Driver<Color, Material::PAWN, Moves, Rules, Board> PawnMoves;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "BothMoves.hpp"
