#include "Value.h"
#include <sstream>

bool TreeSearch::Value::is_loss(int score)
{
        return LOSS_0 <= score && score <= LOSS_MAX;
}

bool TreeSearch::Value::is_win(int score)
{
        return WIN_MAX <= score && score <= WIN_1;
}

bool TreeSearch::Value::is_infinity(int score)
{
        return score == infinity();
}

bool TreeSearch::Value::is_heuristic(int score)
{
        return !(is_loss(score) || is_win(score)) ;
}

bool TreeSearch::Value::is_finite(int score)
{
        return -infinity() < score && score < infinity();
}

int TreeSearch::Value::loss(int ply)
{
        return LOSS_0 + ply / 2;
}

int TreeSearch::Value::draw(void)
{
        return DRAW;
}

int TreeSearch::Value::win(int ply)
{
        return WIN_1 - ply / 2;
}

int TreeSearch::Value::infinity(void)
{
        return INFINITY;
}

int TreeSearch::Value::squeeze(int score)
{
        return score - is_win(score);
}

int TreeSearch::Value::stretch(int score)
{
        return score - is_loss(score);
}

std::string TreeSearch::Value::print(int score)
{
        std::stringstream sstr;

        if (is_infinity(-score))
                sstr << "-INF";
        else if (is_loss(score))
                sstr << "L" << 2 * (score - LOSS_0);
        else if (is_win(score))
                sstr << "W" << 2 * (WIN_1 - score) + 1;
        else if (is_infinity(score))
                sstr << "+INF";
        else
                sstr << score;

        return sstr.str();
}
