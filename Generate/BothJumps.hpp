#include "KingJumps.h"
#include "PawnJumps.h"
#include "Propagate.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::BOTH, Move::JUMPS, Color, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_precede(p, moves);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::BOTH, Move::JUMPS, Color, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        generate_precede(p, moves, Int2Type<is_AbsoluteKingPrecedence<Rules>::VALUE>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::BOTH, Move::JUMPS, Color, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<false>)
{
        GenerateDriver<Pieces::KING, Move::JUMPS, Color, Rules, Board>::generate(p, moves);
        GenerateDriver<Pieces::PAWN, Move::JUMPS, Color, Rules, Board>::generate(p, moves);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::BOTH, Move::JUMPS, Color, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<true>)
{
        GenerateDriver<Pieces::KING, Move::JUMPS, Color, Rules, Board>::generate(p, moves);
        if (!moves.size())
                GenerateDriver<Pieces::PAWN, Move::JUMPS, Color, Rules, Board>::generate(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Pieces::BOTH, Move::JUMPS, Color, Rules, Board>::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate(p, captures);
        return captures.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Pieces::BOTH, Move::JUMPS, Color, Rules, Board>::detect(const Position<Board>& p)
{
        return GenerateDriver<Pieces::PAWN, Move::JUMPS, Color, Rules, Board>::detect(p) || GenerateDriver<Pieces::KING, Move::JUMPS, Color, Rules, Board>::detect(p);
}
