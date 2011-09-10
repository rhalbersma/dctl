#include "Driver.h"
#include "Selection.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "KingReverse.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "PawnReverse.h"
#include "Promotion.h"

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Rules, typename Board> 
void State<Color, Material, Selection, Rules, Board>::do_generate(const Position<Board>& p, Stack& moves) const 
{ 
        Driver<Color, Material, Selection, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
int State<Color, Material, Selection, Rules, Board>::do_count(const Position<Board>& p) const
{
        return Driver<Color, Material, Selection, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Selection, typename Rules, typename Board> 
bool State<Color, Material, Selection, Rules, Board>::do_detect(const Position<Board>& p) const 
{ 
        return Driver<Color, Material, Selection, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> 
void State<Color, Material, Legal, Rules, Board>::do_generate(const Position<Board>& p, Stack& moves) const 
{
        Driver<Color, Material, Jumps, Rules, Board>::generate(p, moves);
        if (moves.empty())
                Driver<Color, Material, Moves, Rules, Board>::generate(p, moves);
}

template<bool Color, int Material, typename Rules, typename Board>
int State<Color, Material, Legal, Rules, Board>::do_count(const Position<Board>& p) const
{
        auto num_moves = Driver<Color, Material, Jumps, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += Driver<Color, Material, Moves, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, int Material, typename Rules, typename Board> 
bool State<Color, Material, Legal, Rules, Board>::do_detect(const Position<Board>& p) const 
{ 
        return (
                Driver<Color, Material, Moves, Rules, Board>::detect(p) || 
                Driver<Color, Material, Jumps, Rules, Board>::detect(p)
        );
}

}       // namespace successor
}       // namespace dctl
