#pragma once
#include "Driver_fwd.hpp"
#include "PawnMoves.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Stack_fwd.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for reverse pawn moves
template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::pawn, select::Reverse, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
private:
        // typedefs
        typedef Driver<!Color, Material::pawn, select::Moves, Rules, Board> PassivePawnMoves;

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
};

}       // namespace successor
}       // namespace dctl
