#include <climits>
#include <sstream>
#include "Score.h"
#include "../utils/Ply.h"

//namespace {


//}       // namespace

namespace dctl {
const int INFINITY = SHRT_MAX;
const int LOSS_0 = -(INFINITY - 1);
const int LOSS_THRESHOLD = LOSS_0 + MAX_GRAFTED_PLY / 2;
const int WIN_1 = -LOSS_0;
const int WIN_THRESHOLD = -LOSS_THRESHOLD;
const int DRAW = 0;
namespace search {

bool score::is_infinite(int value)
{
        return !is_finite(value);
}

bool score::is_finite(int value)
{
        return value > -infinity() && value < infinity();
}

bool score::is_mate(int value)
{
        return is_loss(value) || is_win(value);
}

bool score::is_loss(int value)
{
        return value < LOSS_THRESHOLD && value >= LOSS_0;
}

bool score::is_win(int value)
{
        return value > WIN_THRESHOLD && value <= WIN_1;
}

int score::infinity()
{
        return INFINITY;
}

int score::loss_value(int ply)
{
        return LOSS_0 + ply / 2;
}

int score::win_value(int ply)
{
        return WIN_1 - ply / 2;
}

int score::loss_ply(int value)
{
        return 2 * (value - LOSS_0);
}

int score::win_ply(int value)
{
        return 2 * (WIN_1 - value) + 1;
}

int score::draw()
{
        return DRAW;
}

int score::stretch(int value)
{
        return value - is_loss(value);
}

int score::squeeze(int value)
{
        return value - is_win(value);
}

std::string score::print(int value)
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