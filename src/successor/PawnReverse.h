#pragma once
#include "PawnMoves.h"
#include "../node/Material.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
template<typename> class Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Reverse;
class Moves;

// partial specialization for reverse pawn moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Reverse, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // typedefs
        typedef Driver<!Color, Material::PAWN, Moves, Rules, Board> Regular;

        // implementation
        static void generate_dirs(BitBoard, BitBoard, Stack&);
        static int count_dirs(BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "PawnReverse.hpp"