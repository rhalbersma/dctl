#include "SameKingMoves.h"
#include "../../Utilities/Bit.h"

BitBoard& SameKingMoves::king(void)
{
        return d_king.back();
}

BitBoard SameKingMoves::king(void) const
{
        return d_king.back();
}

PlyCount& SameKingMoves::moves(void)
{
        return d_moves.back();
}

PlyCount SameKingMoves::moves(void) const
{
        return d_moves.back();
}

void SameKingMoves::copy_back(void)
{
        d_king.copy_back();
        d_moves.copy_back();
}

void SameKingMoves::push_back(BitBoard s_king, PlyCount s_moves)
{
        d_king.push_back(s_king);
        d_moves.push_back(s_moves);
}

void SameKingMoves::pop_back(void)
{
        d_king.pop_back();
        d_moves.pop_back();
}

void SameKingMoves::init(BitBoard s_king)
{
        king() = s_king;
        moves() = 1;

        assert(invariant());
}

void SameKingMoves::increment(BitBoard s_king)
{
        assert(Bit::is_double(s_king));

        king() ^= s_king;
        ++moves();

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
        return !Bit::is_multiple(king()) && ((king() ==  0) == (moves() == 0));
}
