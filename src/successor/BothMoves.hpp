#include <cassert>
#include "KingMoves.h"
#include "PawnMoves.h"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::generate_regular(const Position<Board>& p, Stack& moves)
{
        Driver<Color, Material::KING, Move::MOVES, Rules, Board>::generate_regular(p, moves);
        Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::generate_regular(p, moves);     
}

template<bool Color, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::generate_reverse(const Position<Board>& p, Stack& moves)
{
        Driver<Color, Material::KING, Move::MOVES, Rules, Board>::generate_reverse(p, moves);
        Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::generate_reverse(p, moves);     
}

template<bool Color, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::generate_promotions(const Position<Board>& p, Stack& moves)
{
        Driver<Color, Material::KING, Move::MOVES, Rules, Board>::generate_promotions(p, moves);
        Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::generate_promotions(p, moves);     
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::count_regular(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::count_regular(p) + 
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::count_regular(p)
        );
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::count_reverse(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::count_reverse(p) + 
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::count_reverse(p)
        );
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::count_promotions(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::count_promotions(p) + 
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::count_promotions(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::detect_regular(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::detect_regular(p) || 
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::detect_regular(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::detect_reverse(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::detect_reverse(p) || 
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::detect_reverse(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::detect_promotions(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::detect_promotions(p) || 
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::detect_promotions(p)
        );
}

}       // namespace successor
}       // namespace dctl
