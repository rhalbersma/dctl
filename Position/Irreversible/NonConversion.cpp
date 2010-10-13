#include "NonConversion.h"

PlyCount& NonConversion::moves(void)
{
        return d_moves.back();
}

PlyCount NonConversion::moves(void) const
{
        return d_moves.back();
}

void NonConversion::copy_back(void)
{
        d_moves.copy_back();
}

void NonConversion::push_back(PlyCount s_moves)
{
        d_moves.push_back(s_moves);
}

void NonConversion::pop_back(void)
{
        d_moves.pop_back();
}

void NonConversion::increment(void)
{
        ++moves();
}

void NonConversion::reset(void)
{
        moves() = 0;
}
