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
struct Driver<Color, Material::both, Reverse, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                KingReverse::generate(p, moves);
                PawnReverse::generate(p, moves);
        }

        static int count(const Position<Board>& p)
        {     
                return (
                        KingReverse::count(p) + 
                        PawnReverse::count(p)
                );
        }

        static bool detect(const Position<Board>& p)
        {
                return (
                        PawnReverse::detect(p) || 
                        KingReverse::detect(p)
                );
        }

private:
        // typedefs
        typedef Driver<Color, Material::king, Reverse, Rules, Board> KingReverse;
        typedef Driver<Color, Material::pawn, Reverse, Rules, Board> PawnReverse;
};

}       // namespace successor
}       // namespace dctl
