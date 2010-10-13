#include "Side.h"

// initialize with black to move
Side::Side(void)
: 
        d_side(BLACK)
{
}

// initialize with a side to move
Side::Side(bool to_move)
: 
        d_side(to_move)
{
}

// the side to move
bool Side::to_move(void) const
{
        return d_side;
}

// toggle the side to move
void Side::pass(void)
{
        d_side ^= PASS;
}