#include "KingJumps.h"
#include "PawnJumps.h"
#include "Propagate.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, MoveList& moves)
{
        Propagate<Rules, Board> capture(p);
        generate(p, capture, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves)
{
        generate_precede(p, capture, moves);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves)
{
        generate_precede(p, capture, moves, Int2Type<Variant::is_AbsoluteKingPrecedence<Rules>::VALUE>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<false>)
{
        GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, moves);
        GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, moves);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<true>)
{
        GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, moves);
        if (!moves.size())
                GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        MoveList moves;
        generate(p, moves);
        return moves.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(p) || 
                GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(p)
        );
}
