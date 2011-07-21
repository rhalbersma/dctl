#include "Driver.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "PawnJumps.h"
#include "PawnMoves.h"

namespace dtl {
namespace generate {

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate(const Position<Board>& p, Stack& move_stack) const 
{
        Driver<Color, Material, JUMPS, Rules, Board>::generate(p, move_stack);
        if (move_stack.empty())
                Driver<Color, Material, MOVES, Rules, Board>::generate(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate_captures(const Position<Board>& p, Stack& move_stack) const 
{ 
        Driver<Color, Material, JUMPS, Rules, Board>::generate(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate_reverse(const Position<Board>& p, Stack& move_stack) const 
{ 
        Driver<Color, Material, MOVES, Rules, Board>::generate_reverse(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate_promotions(const Position<Board>& p, Stack& move_stack) const 
{ 
        Driver<Color, Material, MOVES, Rules, Board>::generate_promotions(p, move_stack);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t TemplateMethod<Color, Material, Rules, Board>::count(const Position<Board>& p) const
{
        size_t num_moves = Driver<Color, Material, JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += Driver<Color, Material, MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, int Material, typename Rules, typename Board>
size_t TemplateMethod<Color, Material, Rules, Board>::count_captures(const Position<Board>& p) const
{
        return Driver<Color, Material, JUMPS, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t TemplateMethod<Color, Material, Rules, Board>::count_reverse(const Position<Board>& p) const
{
        return Driver<Color, Material, MOVES, Rules, Board>::count_reverse(p);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t TemplateMethod<Color, Material, Rules, Board>::count_promotions(const Position<Board>& p) const
{
        return Driver<Color, Material, MOVES, Rules, Board>::count_promotions(p);
}

template<bool Color, int Material, typename Rules, typename Board>
size_t TemplateMethod<Color, Material, Rules, Board>::count_mobility(const Position<Board>& p) const
{
        return Driver<Color, Material, MOVES, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return (
                Driver<Color, Material, MOVES, Rules, Board>::detect(p) || 
                Driver<Color, Material, JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return Driver<Color, Material, JUMPS, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect_reverse(const Position<Board>& p) const 
{
        return Driver<Color, Material, MOVES, Rules, Board>::detect_reverse(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect_promotions(const Position<Board>& p) const 
{
        return Driver<Color, Material, MOVES, Rules, Board>::detect_promotions(p);
}

}       // namespace generate
}       // namespace dtl
