#include "GenerateDriver.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "Propagate.h"
#include "../Utilities/InlineOptions.h"

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE 
void GeneratePiece<Piece, Color, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves) const 
{ 
        GenerateDriver<Piece, Move::JUMPS, Color, Rules, Board>::generate(p, moves);
        if (!moves.size())
                GenerateDriver<Piece, Move::MOVES, Color, Rules, Board>::generate(p, moves);
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE 
void GeneratePiece<Piece, Color, Rules, Board>::generate_captures(const Position<Board>& p, Propagate<Rules, Board>& moves) const 
{ 
        GenerateDriver<Piece, Move::JUMPS, Color, Rules, Board>::generate(p, moves);
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE 
void GeneratePiece<Piece, Color, Rules, Board>::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves) const 
{ 
        GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::generate_promotions(p, moves);
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GeneratePiece<Piece, Color, Rules, Board>::count(const Position<Board>& p) const
{
        size_t num_moves = GenerateDriver<Piece, Move::JUMPS, Color, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += GenerateDriver<Piece, Move::MOVES, Color, Rules, Board>::count(p);
        return num_moves;
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GeneratePiece<Piece, Color, Rules, Board>::count_moves(const Position<Board>& p) const
{
        return GenerateDriver<Piece, Move::MOVES, Color, Rules, Board>::count(p);
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE 
bool GeneratePiece<Piece, Color, Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return GenerateDriver<Piece, Move::MOVES, Color, Rules, Board>::detect(p) || GenerateDriver<Piece, Move::JUMPS, Color, Rules, Board>::detect(p);
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE 
bool GeneratePiece<Piece, Color, Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return GenerateDriver<Piece, Move::JUMPS, Color, Rules, Board>::detect(p);
}

template<Pieces::Composition Piece, bool Color, typename Rules, typename Board> FORCE_INLINE 
bool GeneratePiece<Piece, Color, Rules, Board>::detect_promotions(const Position<Board>& p) const 
{ 
        return GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::detect_promotions(p);
}
