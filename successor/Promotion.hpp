#include <boost/assert.hpp>
#include "../node/Position.h"
#include "../node/Promotion.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Promotion, Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        ActivePawnMoves::generate_dirs(promoting_men<Color, Board>(p.men(Color)), not_occupied(p), moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Promotion, Rules, Board>::count(const Position<Board>& p)
{
        return ActivePawnMoves::count_dirs(promoting_men<Color, Board>(p.men(Color)), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Promotion, Rules, Board>::detect(const Position<Board>& p)
{
        return ActivePawnMoves::detect_dirs(promoting_men<Color, Board>(p.men(Color)), not_occupied(p));
}

}       // namespace successor
}       // namespace dctl
