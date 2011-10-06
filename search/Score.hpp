#pragma once
#include <climits>                      // SHRT_MAX
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include "../utility/Ply.hpp"

namespace dctl {
namespace search {

enum {
        INFINITY = SHRT_MAX,
        LOSS_0 = -(INFINITY - 1),
        LOSS_THRESHOLD = LOSS_0 + MAX_MATE_MOVES / 2,
        WIN_1 = -LOSS_0,
        WIN_THRESHOLD = -LOSS_THRESHOLD,
        DRAW = 0
};

// views
inline int infinity()
{
        return INFINITY;
}

inline int loss_value(int ply)
{
        return LOSS_0 + ply / 2;
}

inline int win_value(int ply)
{
        return WIN_1 - ply / 2;
}

inline int loss_ply(int value)
{
        return 2 * (value - LOSS_0);
}

inline int win_ply(int value)
{
        return 2 * (WIN_1 - value) + 1;
}

inline int draw()
{
        return DRAW;
}

// predicates
inline bool is_finite(int value)
{
        return value > -infinity() && value < infinity();
}

inline bool is_infinite(int value)
{
        return !is_finite(value);
}

inline bool is_loss(int value)
{
        return value >= LOSS_0 && value < LOSS_THRESHOLD;
}

inline bool is_win(int value)
{
        return value > WIN_THRESHOLD && value <= WIN_1;
}

inline bool is_mate(int value)
{
        return is_loss(value) || is_win(value);
}

// modifiers
inline int stretch(int value)
{
        return value - is_loss(value);
}

inline int squeeze(int value)
{
        return value - is_win(value);
}

inline std::string print(int value)
{
        std::stringstream sstr;

        if (is_infinite(value))
                sstr << (value < 0? "-" : "+") << "INF";
        else if (is_loss(value))
                sstr << "L" << loss_ply(value);
        else if (is_win(value))
                sstr << "W" << win_ply(value);
        else
                sstr << value;

        return sstr.str();
}

}       // namespace search
}       // namespace dctl
