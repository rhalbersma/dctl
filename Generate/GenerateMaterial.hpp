#include "GenerateTemplate.h"
#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "../Utilities/InlineOptions.h"

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE 
void GenerateMaterial<Color, Material, Rules, Board>::generate(const Position<Board>& p, MoveList& moves) const 
{ 
        GenerateTemplate<Color, Material, Move::JUMPS, Rules, Board>::generate(p, moves);
        if (!moves.size())
                GenerateTemplate<Color, Material, Move::MOVES, Rules, Board>::generate(p, moves);
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE 
void GenerateMaterial<Color, Material, Rules, Board>::generate_captures(const Position<Board>& p, MoveList& moves) const 
{ 
        GenerateTemplate<Color, Material, Move::JUMPS, Rules, Board>::generate(p, moves);
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE 
void GenerateMaterial<Color, Material, Rules, Board>::generate_promotions(const Position<Board>& p, MoveList& moves) const 
{ 
        GenerateTemplate<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate_promotions(p, moves);
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE
size_t GenerateMaterial<Color, Material, Rules, Board>::count(const Position<Board>& p) const
{
        size_t num_moves = GenerateTemplate<Color, Material, Move::JUMPS, Rules, Board>::count(p);
        if (!num_moves)
                num_moves += GenerateTemplate<Color, Material, Move::MOVES, Rules, Board>::count(p);
        return num_moves;
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE
size_t GenerateMaterial<Color, Material, Rules, Board>::count_mobility(const Position<Board>& p) const
{
        return GenerateTemplate<Color, Material, Move::MOVES, Rules, Board>::count(p);
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE 
bool GenerateMaterial<Color, Material, Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return (
                GenerateTemplate<Color, Material, Move::MOVES, Rules, Board>::detect(p) || 
                GenerateTemplate<Color, Material, Move::JUMPS, Rules, Board>::detect(p)
        );
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE 
bool GenerateMaterial<Color, Material, Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return GenerateTemplate<Color, Material, Move::JUMPS, Rules, Board>::detect(p);
}

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> FORCE_INLINE 
bool GenerateMaterial<Color, Material, Rules, Board>::detect_promotions(const Position<Board>& p) const 
{
        return (Material == Pieces::KING)? false : GenerateTemplate<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect_promotions(p);
}
