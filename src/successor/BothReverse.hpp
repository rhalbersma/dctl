#pragma once
#include "Driver_fwd.hpp"
#include "KingReverse.hpp"
#include "PawnReverse.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Reverse, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::king, select::Reverse, Rules, Board> KingReverse;
        typedef Driver<Color, Material::pawn, select::Reverse, Rules, Board> PawnReverse;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                KingReverse::generate(p, moves);
                PawnReverse::generate(p, moves);
        }

        template<typename Position>
        static int count(Position const& p)
        {
                return (
                        KingReverse::count(p) +
                        PawnReverse::count(p)
                );
        }

        template<typename Position>
        static bool detect(Position const& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnReverse::detect(p) ||
                        KingReverse::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
