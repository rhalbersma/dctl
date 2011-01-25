#pragma once
#include "../IO/Token.h"

struct DXP_tag {};

template<>
struct PositionToken<DXP_tag>: public PositionTokenTriple<'Z', 'W', 'e'>
{
        static bool read_color(char);
        static bool read_setup(char);

        static char write_color(bool);
        static char write_setup(bool);

        static const char INITIAL = 'A';
        static const char SPECIAL = 'B';
        static const char SETUP[2];

        static const char BLACK_KING = 'Z';
        static const char BLACK_MAN  = 'z';
        static const char WHITE_KING = 'W';
        static const char WHITE_MAN  = 'w';
};
