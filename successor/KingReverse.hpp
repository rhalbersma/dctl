#pragma once
#include "../node/Move.hpp"
#include "../node/Stack.hpp"
#include "../utility/NonConstructible.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

namespace board { template<bool, typename> struct Direction; }
template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
struct Reverse;

// partial specialization for reverse king moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::king, Reverse, Rules, Board>
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
        typedef Driver<Color, Material::king, Moves, Rules, Board> ActiveKingMoves;
};

}       // namespace successor
}       // namespace dctl
