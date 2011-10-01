#pragma once
#include "../node/Material.h"
#include "../node/Position.h"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

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
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                PassivePawnMoves::generate_dirs(p.men(Color), not_occupied(p), moves);
        }

        static int count(const Position<Board>& p)
        {
                return PassivePawnMoves::count_dirs(p.men(Color), not_occupied(p));
        }

        static bool detect(const Position<Board>& p)
        {
                return PassivePawnMoves::detect_dirs(p.men(Color), not_occupied(p));
        }

private:
        // typedefs
        typedef Driver<!Color, Material::PAWN, Moves, Rules, Board> PassivePawnMoves;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "PawnReverse.hpp"
