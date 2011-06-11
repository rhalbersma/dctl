#include <cassert>
#include "KingMoves.h"
#include "PawnMoves.h"

namespace tree {
namespace generate {

template<bool Color, typename Rules, typename Board> 
void Template<Color, node::Pieces::BOTH, move::MOVES, Rules, Board>::generate(const node::Position<Board>& p, move::Stack& move_stack)
{
        Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>::generate(p, move_stack);
        Template<Color, node::Pieces::PAWN, move::MOVES, Rules, Board>::generate(p, move_stack);     
}

template<bool Color, typename Rules, typename Board> 
void Template<Color, node::Pieces::BOTH, move::MOVES, Rules, Board>::generate_promotions(const node::Position<Board>& p, move::Stack& move_stack)
{
        Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>::generate_promotions(p, move_stack);
        Template<Color, node::Pieces::PAWN, move::MOVES, Rules, Board>::generate_promotions(p, move_stack);     
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, node::Pieces::BOTH, move::MOVES, Rules, Board>::count(const node::Position<Board>& p)
{     
        return (
                Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>::count(p) + 
                Template<Color, node::Pieces::PAWN, move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, node::Pieces::BOTH, move::MOVES, Rules, Board>::count_promotions(const node::Position<Board>& p)
{     
        return (
                Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>::count_promotions(p) + 
                Template<Color, node::Pieces::PAWN, move::MOVES, Rules, Board>::count_promotions(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::BOTH, move::MOVES, Rules, Board>::detect(const node::Position<Board>& p)
{
        return (
                Template<Color, node::Pieces::PAWN, move::MOVES, Rules, Board>::detect(p) || 
                Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>::detect(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::BOTH, move::MOVES, Rules, Board>::detect_promotions(const node::Position<Board>& p)
{
        return (
                Template<Color, node::Pieces::PAWN, move::MOVES, Rules, Board>::detect_promotions(p) || 
                Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>::detect_promotions(p)
        );
}

}       // namespace generate
}       // namespace tree
