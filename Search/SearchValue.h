#pragma once
#include "../Utilities/Ply.h"
#include <climits>
#include <string>

class SearchValue
{
public:
        // predicates
        static bool is_loss(int);
        static bool is_win(int);
        static bool is_infinity(int);
        static bool is_heuristic(int);
        static bool is_finite(int);

        // views
        static int loss(int);
        static int draw(void);
        static int win(int);
        static int infinity(void);

        // modifiers
        static int squeeze(int);
        static int stretch(int);

        static std::string print(int);

private:
        static const int INFINITY = SHRT_MAX;
        static const int LOSS_0 = -(INFINITY - 1);
        static const int LOSS_MAX = LOSS_0 + PLY_MAX / 2;
        static const int DRAW = 0;
        static const int WIN_MAX = -LOSS_MAX;
        static const int WIN_1 = -LOSS_0;
};
