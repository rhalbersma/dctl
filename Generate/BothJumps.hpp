#include "KingJumps.h"
#include "PawnJumps.h"
#include "Capture/State.h"
#include <cassert>

template<bool Color, typename Rules, typename Board>
void Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Move::Stack& move_stack)
{
        Capture::State<Rules, Board> capture(p);
        generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack)
{
        generate_precede(p, capture, move_stack);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack)
{
        generate_precede(p, capture, move_stack, Int2Type<Variants::is_absolute_king_precedence<Rules>::value>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack, Int2Type<false>)
{
        Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
        Template<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack, Int2Type<true>)
{
        Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
        if (!move_stack.size())
                Template<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        Move::Stack move_stack;
        generate(p, move_stack);
        return move_stack.size();
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                Template<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(p) || 
                Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(p)
        );
}
