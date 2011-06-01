#include <cassert>
#include "KingJumps.h"
#include "PawnJumps.h"
#include "Capture/State.h"

namespace Tree {
namespace Generate {

template<bool Color, typename Rules, typename Board>
void Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Node::Position<Board>& p, Move::Stack& move_stack)
{
        Capture::State<Rules, Board> capture(p);
        generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::generate(const Node::Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack)
{
        generate_precede(p, capture, move_stack);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Node::Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack)
{
        generate_precede(p, capture, move_stack, Int2Type<Variants::is_absolute_king_precedence<Rules>::value>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Node::Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack, Int2Type<false>)
{
        Template<Color, Node::Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
        Template<Color, Node::Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Node::Position<Board>& p, Capture::State<Rules, Board>& capture, Move::Stack& move_stack, Int2Type<true>)
{
        Template<Color, Node::Pieces::KING, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
        if (!move_stack.size())
                Template<Color, Node::Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::count(const Node::Position<Board>& p)
{
        Move::Stack move_stack;
        generate(p, move_stack);
        return move_stack.size();
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>::detect(const Node::Position<Board>& p)
{
        return (
                Template<Color, Node::Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(p) || 
                Template<Color, Node::Pieces::KING, Move::JUMPS, Rules, Board>::detect(p)
        );
}

}       // namespace Generate
}       // namespace Tree
