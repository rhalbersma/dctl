#include "KingJumps.h"
#include "PawnJumps.h"
#include "Propagate.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_precede(p, moves);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        generate_precede(p, moves, Int2Type<is_AbsoluteKingPrecedence<Rules>::VALUE>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<false>)
{
        GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, moves);
        GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, moves);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<true>)
{
        GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, moves);
        if (!moves.size())
                GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate(p, captures);
        return captures.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(p) || 
                GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(p)
        );
}
