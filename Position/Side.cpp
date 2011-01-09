#include "Side.h"

// initialize with black to move
Side::Side(void)
: 
        side_(BLACK)
{
}

// initialize with a side to move
Side::Side(bool to_move)
: 
        side_(to_move)
{
}

// the side to move
bool Side::to_move(void) const
{
        return side_;
}

// toggle the side to move
void Side::pass(void)
{
        side_ ^= PASS;
}