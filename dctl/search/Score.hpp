#pragma once
#include <climits>                      // SHRT_MAX
#include <sstream>                      // stringstream
#include <string>                       // string
#include <dctl/utility/ply.hpp>

namespace dctl {
namespace search {

// queries
inline
int infinity()
{
        return (SHRT_MAX);
}

inline
int loss_min()
{
        return (-(infinity() - 1));
}

inline
int loss_max()
{
        return (loss_min() + MAX_MATE_MOVES);
}

inline
int win_min()
{
        return (-loss_min());
}

inline
int win_max()
{
        return (-loss_max());
}

inline
int loss_value(int ply)
{
        return (loss_min() + ply);
}

inline
int win_value(int ply)
{
        return (win_min() - ply);
}

inline
int loss_ply(int value)
{
        return (value - loss_min());
}

inline
int win_ply(int value)
{
        return (win_min() - value);
}

inline
int draw_value()
{
        return (0);
}

// predicates
inline
bool is_finite(int value)
{
        return (-infinity() < value && value < infinity());
}

inline
bool is_infinite(int value)
{
        return (!is_finite(value));
}

inline
bool is_loss(int value)
{
        return (loss_min() <= value && value < loss_max());
}

inline
bool is_win(int value)
{
        return (win_max() < value && value <= win_min());
}

inline
bool is_mate(int value)
{
        return (is_loss(value) || is_win(value));
}

// modifiers

// loss and win values are "stretched" one step towards the edges of the [-INF, +INF] interval
inline
int stretch(int value)
{
        return (value - is_loss(value) + is_win(value));
}

// loss and win values are "squeezed" one step towards the center of the [-INF, +INF] interval
inline
int squeeze(int value)
{
        return (value + is_loss(value) - is_win(value));
}

inline
std::string print(int value)
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

        return (sstr.str());
}

}       // namespace search
}       // namespace dctl
