#include <cassert>
#include "KingJumps.h"
#include "PawnJumps.h"
#include "Capture/State.h"
#include "../rules/Rules.h"

namespace dtl {
namespace generate {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::generate(const Position<Board>& p, Stack& move_stack)
{
        capture::State<Rules, Board> capture(p);
        generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::generate(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& move_stack)
{
        generate_precede(p, capture, move_stack);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& move_stack)
{
        generate_precede(p, capture, move_stack, Int2Type<rules::is_absolute_king_precedence<Rules>::value>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& move_stack, Int2Type<false>)
{
        Driver<Color, Pieces::KING, JUMPS, Rules, Board>::generate(p, capture, move_stack);
        Driver<Color, Pieces::PAWN, JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& move_stack, Int2Type<true>)
{
        Driver<Color, Pieces::KING, JUMPS, Rules, Board>::generate(p, capture, move_stack);
        if (move_stack.empty())
                Driver<Color, Pieces::PAWN, JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        Stack move_stack;
        generate(p, move_stack);
        return move_stack.size();
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Pieces::BOTH, JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                Driver<Color, Pieces::PAWN, JUMPS, Rules, Board>::detect(p) || 
                Driver<Color, Pieces::KING, JUMPS, Rules, Board>::detect(p)
        );
}

}       // namespace generate
}       // namespace dtl
