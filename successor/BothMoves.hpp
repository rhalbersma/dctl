#pragma once
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> struct Driver;

template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::both, Moves, Rules, Board>
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
