#include "KingJumps.h"
#include "PawnJumps.h"
#include "Capture/State.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, MoveList& move_list)
{
        Capture::State<Rules, Board> capture(p);
        generate(p, capture, move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Capture::State<Rules, Board>& capture, MoveList& move_list)
{
        generate_precede(p, capture, move_list);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, MoveList& move_list)
{
        generate_precede(p, capture, move_list, Int2Type<Variant::is_AbsoluteKingPrecedence<Rules>::VALUE>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, MoveList& move_list, Int2Type<false>)
{
        GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, move_list);
        GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, move_list);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, MoveList& move_list, Int2Type<true>)
{
        GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, move_list);
        if (!move_list.size())
                GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        MoveList move_list;
        generate(p, move_list);
        return move_list.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(p) || 
                GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(p)
        );
}
