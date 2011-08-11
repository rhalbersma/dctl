#include "Driver.h"

namespace dctl {
namespace generate {

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate(const Position<Board>& p, Stack& moves) const 
{
        Driver<Color, Material, Move::JUMPS, Rules, Board>::generate(p, moves);
        if (moves.empty())
                Driver<Color, Material, Move::MOVES, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate_captures(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::JUMPS, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate_reverse(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::MOVES, Rules, Board>::generate_reverse(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void TemplateMethod<Color, Material, Rules, Board>::generate_promotions(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::MOVES, Rules, Board>::generate_promotions(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board>
int TemplateMethod<Color, Material, Rules, Board>::count(const Position<Board>& p) const
{
        auto num_moves = Driver<Color, Material, Move::JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += Driver<Color, Material, Move::MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, int Material, typename Rules, typename Board>
int TemplateMethod<Color, Material, Rules, Board>::count_captures(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::JUMPS, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board>
int TemplateMethod<Color, Material, Rules, Board>::count_reverse(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::count_reverse(p);
}

template<bool Color, int Material, typename Rules, typename Board>
int TemplateMethod<Color, Material, Rules, Board>::count_promotions(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::count_promotions(p);
}

template<bool Color, int Material, typename Rules, typename Board>
int TemplateMethod<Color, Material, Rules, Board>::count_mobility(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return (
                Driver<Color, Material, Move::MOVES, Rules, Board>::detect(p) || 
                Driver<Color, Material, Move::JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return Driver<Color, Material, Move::JUMPS, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect_reverse(const Position<Board>& p) const 
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::detect_reverse(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool TemplateMethod<Color, Material, Rules, Board>::detect_promotions(const Position<Board>& p) const 
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::detect_promotions(p);
}

}       // namespace generate
}       // namespace dctl
