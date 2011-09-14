#include <boost/assert.hpp>
#include "../node/Position.h"
#include "../board/Direction.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Reverse, Rules, Board>::generate(
        const Position<Board>& p, Stack& moves
)
{
        PassivePawnMoves::generate_dirs(p.men(Color), not_occupied(p), moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Reverse, Rules, Board>::count(
        const Position<Board>& p
)
{
        return PassivePawnMoves::count_dirs(p.men(Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Reverse, Rules, Board>::detect(
        const Position<Board>& p
)
{
        return PassivePawnMoves::detect_dirs(p.men(Color), not_occupied(p));
}

}       // namespace successor
}       // namespace dctl
