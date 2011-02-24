#include "KingMoves.h"
#include "PawnMoves.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE 
void GenerateTemplate<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Move::List& move_list)
{
        GenerateTemplate<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate(p, move_list);
        GenerateTemplate<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate(p, move_list);     
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateTemplate<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                GenerateTemplate<Color, Pieces::KING, Move::MOVES, Rules, Board>::count(p) + 
                GenerateTemplate<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                GenerateTemplate<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect(p) || 
                GenerateTemplate<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect(p)
        );
}
