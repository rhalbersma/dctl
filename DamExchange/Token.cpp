#include "Token.h"
#include "../Position/Side.h"
#include <cassert>

const char PositionToken<DXP_tag>::SETUP[2] = {
        INITIAL,
        SPECIAL
};

bool PositionToken<DXP_tag>::read_color(char c)
{
        switch(c) {
        case BLACK: 
                return Side::BLACK;
        case WHITE: 
                return Side::WHITE;
        default:
                assert(false);
                return Side::BLACK;
        }
}

bool PositionToken<DXP_tag>::read_setup(char c)
{
        switch(c) {                
        case INITIAL: 
                return false;
        case SPECIAL: 
                return true;
        default:
                assert(false);
                return false;
        }
}

char PositionToken<DXP_tag>::write_color(bool b)
{
        return COLOR[b];
}

char PositionToken<DXP_tag>::write_setup(bool b)
{
        return SETUP[b];
}
