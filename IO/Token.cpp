#include "Token.h"
#include "../Position/Reversible/Side.h"
#include <cassert>

const char DXP_PositionToken::SETUP[2] = {
        INITIAL,
        SPECIAL
};

bool DXP_PositionToken::read_color(char c)
{
        switch(c) {
        case DXP_PositionToken::BLACK: return Side::BLACK;
        case DXP_PositionToken::WHITE: return Side::WHITE;
        default:
                assert(false);
                return Side::BLACK;
        }
}

bool DXP_PositionToken::read_setup(char c)
{
        switch(c) {                
        case DXP_PositionToken::INITIAL: return false;
        case DXP_PositionToken::SPECIAL: return true;
        default:
                assert(false);
                return false;
        }
}

char DXP_PositionToken::write_color(bool b)
{
        return DXP_PositionToken::COLOR[b];
}

char DXP_PositionToken::write_setup(bool b)
{
        return DXP_PositionToken::SETUP[b];
}
