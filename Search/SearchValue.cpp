#include "SearchValue.h"
#include <sstream>

bool SearchValue::is_loss(int score)
{
        return LOSS_0 <= score && score <= LOSS_MAX;
}

bool SearchValue::is_win(int score)
{
        return WIN_MAX <= score && score <= WIN_1;
}

bool SearchValue::is_infinity(int score)
{
        return score == infinity();
}

bool SearchValue::is_heuristic(int score)
{
        return !(is_loss(score) || is_win(score)) ;
}

bool SearchValue::is_finite(int score)
{
        return -infinity() < score && score < infinity();
}

int SearchValue::loss(int ply)
{
        return LOSS_0 + ply / 2;
}

int SearchValue::draw(void)
{
        return DRAW;
}

int SearchValue::win(int ply)
{
        return WIN_1 - ply / 2;
}

int SearchValue::infinity(void)
{
        return INFINITY;
}

int SearchValue::squeeze(int score)
{
        return score - is_win(score);
}

int SearchValue::stretch(int score)
{
        return score - is_loss(score);
}

std::string SearchValue::print(int score)
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
