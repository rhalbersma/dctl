#pragma once
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Promotion.hpp"
#include "../node/Stack.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> struct Driver;

// partial specialization for pawn promotions
template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::pawn, Promotion, Rules, Board>
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
        typedef Driver<Color, Material::pawn, Moves, Rules, Board> ActivePawnMoves;
};

}       // namespace successor
}       // namespace dctl
