#include "KingMoves.h"
#include "PawnMoves.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> 
void Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Move::Stack& move_stack)
{
        Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate(p, move_stack);
        Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate(p, move_stack);     
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count(p) + 
                Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect(p) || 
                Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect(p)
        );
}
