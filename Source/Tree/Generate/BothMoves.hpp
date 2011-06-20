#include <cassert>
#include "KingMoves.h"
#include "PawnMoves.h"

namespace tree {
namespace generate {

template<bool Color, typename Rules, typename Board> 
void Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::generate(const node::Position<Board>& p, node::Stack* move_stack)
{
        Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate(p, move_stack);
        Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate(p, move_stack);     
}

template<bool Color, typename Rules, typename Board> 
void Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::generate_reverse(const node::Position<Board>& p, node::Stack* move_stack)
{
        Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_reverse(p, move_stack);
        Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_reverse(p, move_stack);     
}

template<bool Color, typename Rules, typename Board> 
void Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::generate_promotions(const node::Position<Board>& p, node::Stack* move_stack)
{
        Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_promotions(p, move_stack);
        Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_promotions(p, move_stack);     
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::count(const node::Position<Board>& p)
{     
        return (
                Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count(p) + 
                Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::count_reverse(const node::Position<Board>& p)
{     
        return (
                Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_reverse(p) + 
                Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_reverse(p)
        );
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::count_promotions(const node::Position<Board>& p)
{     
        return (
                Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_promotions(p) + 
                Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_promotions(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::detect(const node::Position<Board>& p)
{
        return (
                Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect(p) || 
                Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::detect_reverse(const node::Position<Board>& p)
{
        return (
                Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_reverse(p) || 
                Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect_reverse(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::BOTH, node::MOVES, Rules, Board>::detect_promotions(const node::Position<Board>& p)
{
        return (
                Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_promotions(p) || 
                Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect_promotions(p)
        );
}

}       // namespace generate
}       // namespace tree
