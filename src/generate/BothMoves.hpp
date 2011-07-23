#include <cassert>
#include "KingMoves.h"
#include "PawnMoves.h"

namespace dctl {
namespace generate {

template<bool Color, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Stack& move_stack)
{
        Driver<Color, Material::KING, Move::MOVES, Rules, Board>::generate(p, move_stack);
        Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::generate(p, move_stack);     
}

template<bool Color, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::generate_reverse(const Position<Board>& p, Stack& move_stack)
{
        Driver<Color, Material::KING, Move::MOVES, Rules, Board>::generate_reverse(p, move_stack);
        Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::generate_reverse(p, move_stack);     
}

template<bool Color, typename Rules, typename Board> 
void Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::generate_promotions(const Position<Board>& p, Stack& move_stack)
{
        Driver<Color, Material::KING, Move::MOVES, Rules, Board>::generate_promotions(p, move_stack);
        Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::generate_promotions(p, move_stack);     
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::count(p) + 
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::count(p)
        );
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::count_reverse(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::count_reverse(p) + 
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::count_reverse(p)
        );
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::count_promotions(const Position<Board>& p)
{     
        return (
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::count_promotions(p) + 
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::count_promotions(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::detect(p) || 
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::detect(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::detect_reverse(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::detect_reverse(p) || 
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::detect_reverse(p)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::MOVES, Rules, Board>::detect_promotions(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::MOVES, Rules, Board>::detect_promotions(p) || 
                Driver<Color, Material::KING, Move::MOVES, Rules, Board>::detect_promotions(p)
        );
}

}       // namespace generate
}       // namespace dctl
