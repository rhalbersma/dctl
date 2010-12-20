#include "KingMoves.h"
#include "PawnMoves.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE 
void GenerateDriver<Pieces::BOTH, Move::MOVES, Color, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        GenerateDriver<Pieces::KING, Move::MOVES, Color, Rules, Board>::generate(p, moves);
        GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::generate(p, moves);     
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Pieces::BOTH, Move::MOVES, Color, Rules, Board>::count(const Position<Board>& p)
{     
        return GenerateDriver<Pieces::KING, Move::MOVES, Color, Rules, Board>::count(p) + GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::count(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Pieces::BOTH, Move::MOVES, Color, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::detect(p) || GenerateDriver<Pieces::KING, Move::MOVES, Color, Rules, Board>::detect(p);
}
