#pragma once
#include "../node/Material.h"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
template<typename> class Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Moves;

// partial specialization for pawn moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Moves, Rules, Board>
:
        private utility::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        static void generate_dirs(BitBoard, BitBoard, Stack&);
        static int count_dirs(BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard);

private:
        // typedefs
        typedef board::Direction<Color, Board> Direction;

        // implementation
        template<int> static void generate_dir(BitBoard, BitBoard, Stack&);
        template<int> static int count_dir(BitBoard, BitBoard);
        template<int> static bool detect_dir(BitBoard, BitBoard);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "PawnMoves.hpp"
