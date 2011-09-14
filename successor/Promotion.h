#pragma once
#include "../node/Material.h"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
template<typename> class Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Promotions;
class Moves;

// partial specialization for pawn promotions
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Promotion, Rules, Board>
:
        private utility::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // typedefs
        typedef Driver<Color, Material::PAWN, Moves, Rules, Board> ActivePawnMoves;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "Promotion.hpp"
