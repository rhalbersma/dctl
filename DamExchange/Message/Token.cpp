#include "Token.h"
#include <cassert>

const char PositionToken<DXP_tag>::SETUP[2] = {
        INITIAL,
        SPECIAL
};

bool PositionToken<DXP_tag>::read_color(char c)
{
        switch(c) {
        case BLACK: 
                return false;
        case WHITE: 
                return true;
        default:
                assert(false);
                return false;
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
