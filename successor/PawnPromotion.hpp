#pragma once
#include "Driver_fwd.hpp"
#include "PawnMoves.hpp"
#include "Selection_fwd.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Promotion.hpp"
#include "../node/Stack_fwd.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn promotions
template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::pawn, select::Promotions, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                ActivePawnMoves::generate(promoting_men<Color>(p), not_occupied(p), moves);
        }

        static int count(const Position<Board>& p)
        {
                return ActivePawnMoves::count(promoting_men<Color>(p), not_occupied(p));
        }

        static bool detect(const Position<Board>& p)
        {
                return ActivePawnMoves::detect(promoting_men<Color>(p), not_occupied(p));
        }

private:
        // typedefs
        typedef Driver<Color, Material::pawn, select::Moves, Rules, Board> ActivePawnMoves;
};

}       // namespace successor
}       // namespace dctl
