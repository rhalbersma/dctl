#include <cassert>
#include "KingMoves.h"
#include "PawnMoves.h"

namespace dctl {
namespace successor {

template<bool Color, typename Selection, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Selection, Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        Driver<Color, Material::KING, Selection, Rules, Board>::generate(p, moves);
        Driver<Color, Material::PAWN, Selection, Rules, Board>::generate(p, moves);
}

template<bool Color, typename Selection, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Selection, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Selection, Rules, Board>::count(p) + 
                Driver<Color, Material::PAWN, Selection, Rules, Board>::count(p)
        );
}

template<bool Color, typename Selection, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Selection, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Selection, Rules, Board>::detect(p) || 
                Driver<Color, Material::KING, Selection, Rules, Board>::detect(p)
        );
}

}       // namespace successor
}       // namespace dctl
