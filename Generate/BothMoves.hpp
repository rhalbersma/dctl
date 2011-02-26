#include "KingMoves.h"
#include "PawnMoves.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE 
void Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Move::List& move_list)
{
        Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate(p, move_list);
        Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate(p, move_list);     
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count(p) + 
                Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect(p) || 
                Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect(p)
        );
}
