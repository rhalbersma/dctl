#include <climits>
#include <sstream>
#include "Value.h"
#include "../../Utilities/Ply.h"

namespace {

const int INFINITY = SHRT_MAX;
const int LOSS_0 = -(INFINITY - 1);
const int LOSS_THRESHOLD = LOSS_0 + GRAFTED_PLY_MAX / 2;
const int WIN_1 = -LOSS_0;
const int WIN_THRESHOLD = -LOSS_THRESHOLD;
const int DRAW = 0;

}       // namespace

bool tree::search::value::is_infinite(int score)
{
        return !is_finite(score);
}

bool tree::search::value::is_finite(int score)
{
        return score > -infinity() && score < infinity();
}

bool tree::search::value::is_mate(int score)
{
        return is_loss(score) || is_win(score);
}

bool tree::search::value::is_loss(int score)
{
        return score < LOSS_THRESHOLD && score >= LOSS_0;
}

bool tree::search::value::is_win(int score)
{
        return score > WIN_THRESHOLD && score <= WIN_1;
}

int tree::search::value::infinity(void)
{
        return INFINITY;
}

int tree::search::value::loss(int ply)
{
        return LOSS_0 + ply / 2;
}

int tree::search::value::win(int ply)
{
        return WIN_1 - ply / 2;
}

int tree::search::value::draw(void)
{
        return DRAW;
}

int tree::search::value::stretch(int score)
{
        return score - is_loss(score);
}

int tree::search::value::squeeze(int score)
{
        return score - is_win(score);
}

std::string tree::search::value::print(int score)
{
        std::stringstream sstr;

        if (is_infinite(score))
                sstr << (score < 0? "-" : "+") << "INF";
        else if (is_loss(score))
                sstr << "L" << 2 * (score - LOSS_0);
        else if (is_win(score))
                sstr << "W" << 2 * (WIN_1 - score) + 1;
        else
                sstr << score;

        return sstr.str();
}
