#include "MaterialSelector.h"
#include "../Position/Position.h"
#include "../Utilities/InlineOptions.h"

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateColor<Color, Rules, Board>::generate(const Position<Board>& p, MoveList& moves)
{
        assert(p.to_move() == Color);        
        MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->generate(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateColor<Color, Rules, Board>::generate_captures(const Position<Board>& p, MoveList& moves)
{
        assert(p.to_move() == Color);
        MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->generate_captures(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateColor<Color, Rules, Board>::generate_promotions(const Position<Board>& p, MoveList& moves)
{
        assert(p.to_move() == Color);
        MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->generate_promotions(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateColor<Color, Rules, Board>::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->count(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateColor<Color, Rules, Board>::count_mobility(const Position<Board>& p)
{
        // NO color assertion here because we want to count available moves for both sides
        return MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->count_mobility(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateColor<Color, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->detect(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateColor<Color, Rules, Board>::detect_captures(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->detect_captures(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateColor<Color, Rules, Board>::detect_promotions(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        MaterialSelector<Color, Rules, Board>::select(p.composition(Color))->detect_promotions(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateColor<Color, Rules, Board>::invariant(const Position<Board>& p, size_t num_moves)
{
        assert(p.to_move() == Color);
        return count<Color>(p) == num_moves && detect<Color>(p) == (num_moves > 0);
}
