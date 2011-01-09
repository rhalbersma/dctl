#include "NonConversion.h"

PlyCount& NonConversion::moves(void)
{
        return moves_.back();
}

PlyCount NonConversion::moves(void) const
{
        return moves_.back();
}

void NonConversion::copy_back(void)
{
        moves_.copy_back();
}

void NonConversion::push_back(PlyCount s_moves)
{
        moves_.push_back(s_moves);
}

void NonConversion::pop_back(void)
{
        moves_.pop_back();
}

void NonConversion::increment(void)
{
        ++moves();
}

void NonConversion::reset(void)
{
        moves() = 0;
}
