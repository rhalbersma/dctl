#pragma once
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Promotion.hpp"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

namespace board { template<bool, typename> struct Direction; }
template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
struct Promotions;
struct Moves;

// partial specialization for pawn promotions
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::pawn, Promotion, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                ActivePawnMoves::generate_dirs(promoting_men<Color, Board>(p.men(Color)), not_occupied(p), moves);
        }

        static int count(const Position<Board>& p)
        {
                return ActivePawnMoves::count_dirs(promoting_men<Color, Board>(p.men(Color)), not_occupied(p));
        }

        static bool detect(const Position<Board>& p)
        {
                return ActivePawnMoves::detect_dirs(promoting_men<Color, Board>(p.men(Color)), not_occupied(p));
        }

private:
        // typedefs
        typedef Driver<Color, Material::pawn, Moves, Rules, Board> ActivePawnMoves;
};

}       // namespace successor
}       // namespace dctl
