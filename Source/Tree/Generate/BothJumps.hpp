#include <cassert>
#include "KingJumps.h"
#include "PawnJumps.h"
#include "Capture/State.h"

namespace tree {
namespace generate {

template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::generate(const node::Position<Board>& p, move::Stack& move_stack)
{
        capture::State<Rules, Board> capture(p);
        generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::generate(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack& move_stack)
{
        generate_precede(p, capture, move_stack);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::generate_precede(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack& move_stack)
{
        generate_precede(p, capture, move_stack, Int2Type<variants::is_absolute_king_precedence<Rules>::value>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::generate_precede(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack& move_stack, Int2Type<false>)
{
        Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
        Template<Color, node::Pieces::PAWN, move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::generate_precede(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack& move_stack, Int2Type<true>)
{
        Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
        if (!move_stack.size())
                Template<Color, node::Pieces::PAWN, move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::count(const node::Position<Board>& p)
{
        move::Stack move_stack;
        generate(p, move_stack);
        return move_stack.size();
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>::detect(const node::Position<Board>& p)
{
        return (
                Template<Color, node::Pieces::PAWN, move::JUMPS, Rules, Board>::detect(p) || 
                Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect(p)
        );
}

}       // namespace generate
}       // namespace tree
