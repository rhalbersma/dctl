#include "KingMoves.h"
#include "PawnMoves.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE 
void GenerateDriver<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate(p, moves);
        GenerateDriver<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate(p, moves);     
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::count(p) + 
                GenerateDriver<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::BOTH, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return (
                GenerateDriver<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect(p) || 
                GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect(p)
        );
}
