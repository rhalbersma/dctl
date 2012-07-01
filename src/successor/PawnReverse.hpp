#pragma once
#include "Driver_fwd.hpp"
#include "PawnMoves.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for reverse pawn moves
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Reverse, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<!Color, Material::pawn, select::Moves, Rules, Board> PassivePawnMoves;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                PassivePawnMoves::generate(p.pawns(Color), not_occupied(p), moves);
        }

        template<typename Position>
        static int count(Position const& p)
        {
                return PassivePawnMoves::count(p.pawns(Color), not_occupied(p));
        }

        template<typename Position>
        static bool detect(Position const& p)
        {
                return PassivePawnMoves::detect(p.pawns(Color), not_occupied(p));
        }
};

}       // namespace successor
}       // namespace dctl
