#include "GenerateDriver.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "Propagate.h"
#include "../Utilities/InlineOptions.h"

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE 
void GeneratePiece<Color, Piece, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves) const 
{ 
        GenerateDriver<Color, Piece, Move::JUMPS, Rules, Board>::generate(p, moves);
        if (!moves.size())
                GenerateDriver<Color, Piece, Move::MOVES, Rules, Board>::generate(p, moves);
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE 
void GeneratePiece<Color, Piece, Rules, Board>::generate_captures(const Position<Board>& p, Propagate<Rules, Board>& moves) const 
{ 
        GenerateDriver<Color, Piece, Move::JUMPS, Rules, Board>::generate(p, moves);
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE 
void GeneratePiece<Color, Piece, Rules, Board>::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves) const 
{ 
        GenerateDriver<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate_promotions(p, moves);
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE
size_t GeneratePiece<Color, Piece, Rules, Board>::count(const Position<Board>& p) const
{
        size_t num_moves = GenerateDriver<Color, Piece, Move::JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += GenerateDriver<Color, Piece, Move::MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE
size_t GeneratePiece<Color, Piece, Rules, Board>::count_moves(const Position<Board>& p) const
{
        return GenerateDriver<Color, Piece, Move::MOVES, Rules, Board>::count(p);
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE 
bool GeneratePiece<Color, Piece, Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return (
                GenerateDriver<Color, Piece, Move::MOVES, Rules, Board>::detect(p) || 
                GenerateDriver<Color, Piece, Move::JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE 
bool GeneratePiece<Color, Piece, Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return GenerateDriver<Color, Piece, Move::JUMPS, Rules, Board>::detect(p);
}

template<bool Color, Pieces::Composition Piece, typename Rules, typename Board> FORCE_INLINE 
bool GeneratePiece<Color, Piece, Rules, Board>::detect_promotions(const Position<Board>& p) const 
{ 
        return GenerateDriver<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect_promotions(p);
}
