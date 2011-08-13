#include "Driver.h"

namespace dctl {
namespace successors {

template<bool Color, int Material, typename Rules, typename Board> 
void Generator<Color, Material, Rules, Board>::do_generate(const Position<Board>& p, Stack& moves) const 
{
        Driver<Color, Material, Move::JUMPS, Rules, Board>::generate(p, moves);
        if (moves.empty())
                Driver<Color, Material, Move::MOVES, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void Generator<Color, Material, Rules, Board>::do_generate_moves(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::MOVES, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void Generator<Color, Material, Rules, Board>::do_generate_jumps(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::JUMPS, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void Generator<Color, Material, Rules, Board>::do_generate_reverse(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::MOVES, Rules, Board>::generate_reverse(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board> 
void Generator<Color, Material, Rules, Board>::do_generate_promotions(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Move::MOVES, Rules, Board>::generate_promotions(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board>
int Generator<Color, Material, Rules, Board>::do_count(const Position<Board>& p) const
{
        auto num_moves = Driver<Color, Material, Move::JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += Driver<Color, Material, Move::MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, int Material, typename Rules, typename Board>
int Generator<Color, Material, Rules, Board>::do_count_moves(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board>
int Generator<Color, Material, Rules, Board>::do_count_jumps(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::JUMPS, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board>
int Generator<Color, Material, Rules, Board>::do_count_reverse(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::count_reverse(p);
}

template<bool Color, int Material, typename Rules, typename Board>
int Generator<Color, Material, Rules, Board>::do_count_promotions(const Position<Board>& p) const
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::count_promotions(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool Generator<Color, Material, Rules, Board>::do_detect(const Position<Board>& p) const 
{ 
        return (
                Driver<Color, Material, Move::MOVES, Rules, Board>::detect(p) || 
                Driver<Color, Material, Move::JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, int Material, typename Rules, typename Board> 
bool Generator<Color, Material, Rules, Board>::do_detect_moves(const Position<Board>& p) const 
{ 
        return Driver<Color, Material, Move::MOVES, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool Generator<Color, Material, Rules, Board>::do_detect_jumps(const Position<Board>& p) const 
{ 
        return Driver<Color, Material, Move::JUMPS, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool Generator<Color, Material, Rules, Board>::do_detect_reverse(const Position<Board>& p) const 
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::detect_reverse(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
bool Generator<Color, Material, Rules, Board>::do_detect_promotions(const Position<Board>& p) const 
{
        return Driver<Color, Material, Move::MOVES, Rules, Board>::detect_promotions(p);
}

}       // namespace successors
}       // namespace dctl
