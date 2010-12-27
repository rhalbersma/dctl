#pragma once

static const char WHITE_SPACE = ' ';

template<char B, char W, char E>
struct PositionToken
{
        static const char BLACK = B;
        static const char WHITE = W;
        static const char EMPTY = E;

        static const char COLOR[2];
        static const char LOWER[2];
        static const char UPPER[2];
};

struct FEN_PositionToken: public PositionToken<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '\"';
};

class DXP_PositionToken: public PositionToken<'Z', 'W', 'e'>
{
public:
        static bool read_color(char);
        static bool read_setup(char);

        static char write_color(bool);
        static char write_setup(bool);

        static const char BLACK_KING = 'Z';
        static const char BLACK_MAN  = 'z';
        static const char WHITE_KING = 'W';
        static const char WHITE_MAN  = 'w';

private:
        static const char INITIAL = 'A';
        static const char SPECIAL = 'B';
        static const char SETUP[2];
};

template<char M, char C>
struct MoveToken
{
        static const char MOVE    = M;
        static const char CAPTURE = C;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Token.hpp"
