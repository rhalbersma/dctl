#pragma once
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {

namespace board { template<bool, typename> struct Direction; }
template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
struct Reverse;
struct Moves;

// partial specialization for reverse pawn moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::pawn, Reverse, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                PassivePawnMoves::generate(p.men(Color), not_occupied(p), moves);
        }

        static int count(const Position<Board>& p)
        {
                return PassivePawnMoves::count(p.men(Color), not_occupied(p));
        }

        static bool detect(const Position<Board>& p)
        {
                return PassivePawnMoves::detect(p.men(Color), not_occupied(p));
        }

private:
        // typedefs
        typedef Driver<!Color, Material::pawn, Moves, Rules, Board> PassivePawnMoves;
};

}       // namespace successor
}       // namespace dctl
