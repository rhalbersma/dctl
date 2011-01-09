#include "SameKingMoves.h"
#include "../Utilities/Bit.h"

BitBoard& SameKingMoves::king(void)
{
        return king_.back();
}

BitBoard SameKingMoves::king(void) const
{
        return king_.back();
}

PlyCount& SameKingMoves::moves(void)
{
        return moves_.back();
}

PlyCount SameKingMoves::moves(void) const
{
        return moves_.back();
}

void SameKingMoves::copy_back(void)
{
        king_.copy_back();
        moves_.copy_back();
}

void SameKingMoves::push_back(BitBoard s_king, PlyCount s_moves)
{
        king_.push_back(s_king);
        moves_.push_back(s_moves);
}

void SameKingMoves::pop_back(void)
{
        king_.pop_back();
        moves_.pop_back();
}

void SameKingMoves::init(BitBoard k)
{
        king() = k;
        moves() = 1;

        assert(invariant());
}

void SameKingMoves::increment(BitBoard k)
{
        assert(Bit::is_double(k));

        king() ^= k;
        moves() += 1;

        assert(invariant());
}

void SameKingMoves::reset(void)
{
        king() = 0;
        moves() = 0;

        assert(invariant());
}

bool SameKingMoves::invariant(void) const
{
        return (
                !Bit::is_multiple(king()) && 
                ((king() ==  0) == (moves() == 0))
        );
}
