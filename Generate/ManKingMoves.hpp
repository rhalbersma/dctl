#include "KingMoves.h"
#include "ManMoves.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<typename Rules, typename Board> FORCE_INLINE 
void ManKingMoves::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate<Side::WHITE, Rules>(p, moves);
        else
                generate<Side::BLACK, Rules>(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE 
void ManKingMoves::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        KingMoves::generate<Color, Rules>(p, moves);
        ManMoves::generate<Color>(p, moves);     
}

template<typename Rules, typename Board> FORCE_INLINE
size_t ManKingMoves::count(const Position<Board>& p)
{     
        if (p.to_move())
                return count<Side::WHITE, Rules>(p);
        else
                return count<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t ManKingMoves::count(const Position<Board>& p)
{     
        return KingMoves::count<Color, Rules>(p) + ManMoves::count<Color>(p);
}

template<typename Rules, typename Board> FORCE_INLINE
bool ManKingMoves::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE, Rules>(p);
        else
                return detect<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManKingMoves::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return ManMoves::detect<Color>(p) || KingMoves::detect<Color, Rules>(p);
}
