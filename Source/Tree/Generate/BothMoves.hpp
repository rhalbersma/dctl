#include "KingMoves.h"
#include "PawnMoves.h"
#include <cassert>

namespace Tree {
namespace Generate {

template<bool Color, typename Rules, typename Board> 
void Template<Color, Node::Pieces::BOTH, Move::MOVES, Rules, Board>::generate(const Node::Position<Board>& p, Move::Stack& move_stack)
{
        Template<Color, Node::Pieces::KING, Move::MOVES, Rules, Board>::generate(p, move_stack);
        Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>::generate(p, move_stack);     
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, Node::Pieces::BOTH, Move::MOVES, Rules, Board>::count(const Node::Position<Board>& p)
{     
        return (
                Template<Color, Node::Pieces::KING, Move::MOVES, Rules, Board>::count(p) + 
                Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, Node::Pieces::BOTH, Move::MOVES, Rules, Board>::detect(const Node::Position<Board>& p)
{
        return (
                Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>::detect(p) || 
                Template<Color, Node::Pieces::KING, Move::MOVES, Rules, Board>::detect(p)
        );
}

}       // namespace Generate
}       // namespace Tree
