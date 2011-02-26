#include "Template.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "../Utilities/InlineOptions.h"

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE 
void State<Color, Material, Rules, Board>::generate(const Position<Board>& p, Move::List& move_list) const 
{
        Template<Color, Material, Move::JUMPS, Rules, Board>::generate(p, move_list);
        if (!move_list.size())
                Template<Color, Material, Move::MOVES, Rules, Board>::generate(p, move_list);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE 
void State<Color, Material, Rules, Board>::generate_captures(const Position<Board>& p, Move::List& move_list) const 
{ 
        Template<Color, Material, Move::JUMPS, Rules, Board>::generate(p, move_list);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE 
void State<Color, Material, Rules, Board>::generate_promotions(const Position<Board>& p, Move::List& move_list) const 
{ 
        if (Material != Pieces::KING)
                Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate_promotions(p, move_list);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE
size_t State<Color, Material, Rules, Board>::count(const Position<Board>& p) const
{
        size_t num_moves = Template<Color, Material, Move::JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += Template<Color, Material, Move::MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE
size_t State<Color, Material, Rules, Board>::count_captures(const Position<Board>& p) const
{
        return Template<Color, Material, Move::JUMPS, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE
size_t State<Color, Material, Rules, Board>::count_promotions(const Position<Board>& p) const
{
        return (Material == Pieces::KING)? 0 : Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count_promotions(p);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE
size_t State<Color, Material, Rules, Board>::count_mobility(const Position<Board>& p) const
{
        return Template<Color, Material, Move::MOVES, Rules, Board>::count(p);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE 
bool State<Color, Material, Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return (
                Template<Color, Material, Move::MOVES, Rules, Board>::detect(p) || 
                Template<Color, Material, Move::JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE 
bool State<Color, Material, Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return Template<Color, Material, Move::JUMPS, Rules, Board>::detect(p);
}

template<bool Color, int Material, typename Rules, typename Board> FORCE_INLINE 
bool State<Color, Material, Rules, Board>::detect_promotions(const Position<Board>& p) const 
{
        return (Material == Pieces::KING)? false : Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect_promotions(p);
}
