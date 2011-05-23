#include "Value.h"
#include "../../Utilities/Ply.h"
#include <climits>
#include <sstream>

namespace {

const int INFINITY = SHRT_MAX;
const int LOSS_0 = -(INFINITY - 1);
const int LOSS_MAX = LOSS_0 + PLY_MAX / 2;
const int DRAW = 0;
const int WIN_MAX = -LOSS_MAX;
const int WIN_1 = -LOSS_0;

}       // namespace

bool Tree::Search::Value::is_infinity(int score)
{
        return score == infinity();
}

bool Tree::Search::Value::is_finite(int score)
{
        return score > -infinity() && score < infinity();
}

int Tree::Search::Value::infinity(void)
{
        return INFINITY;
}

bool Tree::Search::Value::is_heuristic(int score)
{
        return !(is_loss(score) || is_win(score)) ;
}

bool Tree::Search::Value::is_loss(int score)
{
        return score >= LOSS_0 && score <= LOSS_MAX;
}

bool Tree::Search::Value::is_win(int score)
{
        return score >= WIN_MAX && score <= WIN_1;
}

int Tree::Search::Value::loss(int ply)
{
        return LOSS_0 + ply / 2;
}

int Tree::Search::Value::win(int ply)
{
        return WIN_1 - ply / 2;
}

int Tree::Search::Value::draw(void)
{
        return DRAW;
}

int Tree::Search::Value::stretch(int score)
{
        return score - is_loss(score);
}

int Tree::Search::Value::squeeze(int score)
{
        return score - is_win(score);
}

std::string Tree::Search::Value::print(int score)
{
        std::stringstream sstr;

        if (is_infinity(-score))
                sstr << "-INF";
        else if (is_infinity(score))
                sstr << "+INF";
        else if (is_loss(score))
                sstr << "L" << 2 * (score - LOSS_0);
        else if (is_win(score))
                sstr << "W" << 2 * (WIN_1 - score) + 1;
        else
                sstr << score;

        return sstr.str();
}
