#include <boost/assert.hpp>

namespace dctl {
namespace successor {

template<bool Color, typename Selection, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Selection, Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        KingMoves::generate(p, moves);
        PawnMoves::generate(p, moves);
}

template<bool Color, typename Selection, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Selection, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                KingMoves::count(p) + 
                PawnMoves::count(p)
        );
}

template<bool Color, typename Selection, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Selection, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                PawnMoves::detect(p) || 
                KingMoves::detect(p)
        );
}

}       // namespace successor
}       // namespace dctl
