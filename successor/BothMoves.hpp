#pragma once
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

template<bool Color, typename Selection, typename Rules, typename Board> 
class Driver<Color, Material::both, Selection, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
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

private:
        // typedefs
        typedef Driver<Color, Material::king, Moves, Rules, Board> KingMoves;
        typedef Driver<Color, Material::pawn, Moves, Rules, Board> PawnMoves;
};

}       // namespace successor
}       // namespace dctl
