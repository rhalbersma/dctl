#pragma once
#include "../node/Material.h"
#include "../node/Stack.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
template<typename> class Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Reverse;

// partial specialization for reverse king moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::KING, Reverse, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                ActiveKingMoves::generate(p, moves);
        }

        static int count(const Position<Board>& p)
        {
                return ActiveKingMoves::count(p);
        }

        static bool detect(const Position<Board>& p)
        {
                return ActiveKingMoves::detect(p);
        }

private:
        // typedefs
        typedef Driver<Color, Material::KING, Moves, Rules, Board> ActiveKingMoves;
};

}       // namespace successor
}       // namespace dctl
