#include "Template.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "PawnJumps.h"
#include "PawnMoves.h"

namespace Tree {
namespace Generate {

template<bool Color, int Material, typename Rules, typename Board> 
void State<Color, Material, Rules, Board>::generate(const Node::Position<Board>& p, Move::Stack& move_stack) const 
{
        Template<Color, Material, Move::JUMPS, Rules, Board>::generate(p, move_stack);
        if (!move_stack.size())
                Template<Color, Material, Move::MOVES, Rules, Board>::generate(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board> 
void State<Color, Material, Rules, Board>::generate_captures(const Node::Position<Board>& p, Move::Stack& move_stack) const 
{ 
        Template<Color, Material, Move::JUMPS, Rules, Board>::generate(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board> 
void State<Color, Material, Rules, Board>::generate_promotions(const Node::Position<Board>& p, Move::Stack& move_stack) const 
{ 
        if (Material != Node::Pieces::KING)
                Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>::generate_promotions(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t State<Color, Material, Rules, Board>::count(const Node::Position<Board>& p) const
{
        size_t num_moves = Template<Color, Material, Move::JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += Template<Color, Material, Move::MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, int Material, typename Rules, typename Board>
size_t State<Color, Material, Rules, Board>::count_captures(const Node::Position<Board>& p) const
{
        return Template<Color, Material, Move::JUMPS, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t State<Color, Material, Rules, Board>::count_promotions(const Node::Position<Board>& p) const
{
        return (Material == Node::Pieces::KING)? 0 : Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>::count_promotions(p);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t State<Color, Material, Rules, Board>::count_mobility(const Node::Position<Board>& p) const
{
        return Template<Color, Material, Move::MOVES, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool State<Color, Material, Rules, Board>::detect(const Node::Position<Board>& p) const 
{ 
        return (
                Template<Color, Material, Move::MOVES, Rules, Board>::detect(p) || 
                Template<Color, Material, Move::JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, int Material, typename Rules, typename Board> 
bool State<Color, Material, Rules, Board>::detect_captures(const Node::Position<Board>& p) const 
{ 
        return Template<Color, Material, Move::JUMPS, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool State<Color, Material, Rules, Board>::detect_promotions(const Node::Position<Board>& p) const 
{
        return (Material == Node::Pieces::KING)? false : Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>::detect_promotions(p);
}

}       // namespace Generate
}       // namespace Tree
