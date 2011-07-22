#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "Weight.h"
#include "../generate/Successors.h"
#include "../node/Position.h"
#include "../node/Side.h"
#include "../rules/Variants.h"

namespace dctl {

template<typename Board>
int Evaluate::evaluate(const Position<Board>& p)
{
        return p.to_move()? delta_evaluate<Side::WHITE>(p) : delta_evaluate<Side::BLACK>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_evaluate(const Position<Board>& p)
{
        return evaluate<Color>(p) - evaluate<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_material(const Position<Board>& p)
{
        return material<Color>(p) - material<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_tempo(const Position<Board>& p)
{
        return tempo<Color>(p) - tempo<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_center(const Position<Board>& p)
{
        return center<Color>(p) - center<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_balance(const Position<Board>& p)
{
        return balance<Color>(p) - balance<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_mobility(const Position<Board>& p)
{
        return mobility<Color>(p) - mobility<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::evaluate(const Position<Board>& p)
{
        int score = 0;
        score += material<Color>(p);
        score += tempo<Color>(p);
        score += center<Color>(p);
        score += balance<Color>(p);
        score += mobility<Color>(p);
        return score;
}

template<bool Color, typename Board>
int Evaluate::material(const Position<Board>& p)
{
        return
        (
                Weight::MATERIAL[0] * bit::count(p.pieces(Color)) +
                Weight::MATERIAL[1] * bit::count(p.kings(Color))
        );
}

template<bool Color, typename Board>
int Evaluate::tempo(const Position<Board>& p)
{
        int score = 0;
        for (auto i = 1; i < Board::HEIGHT; ++i)
                score += Weight::TEMPO[i] * bit::count(p.pieces(Color) & Board::ROW_MASK[Color][i]);
        return score;
}

template<bool Color, typename Board>
int Evaluate::center(const Position<Board>& p)
{
        int score = 0;
        for (auto i = 1; i < Board::WIDTH / 2; ++i)
                score += Weight::CENTER[i] *
                (
                        bit::count(p.pieces(Color) & Board::COL_MASK[ Color][i]) +
                        bit::count(p.pieces(Color) & Board::COL_MASK[!Color][i])
                );
        return score;
}

template<bool Color, typename Board>
int Evaluate::balance(const Position<Board>& p)
{
        int score = 0;
        for (auto i = 0; i < Board::WIDTH / 2; ++i)
                score += Weight::BALANCE[i] *
                (
                        bit::count(p.pieces(Color) & Board::COL_MASK[ Color][i]) -
                        bit::count(p.pieces(Color) & Board::COL_MASK[!Color][i])
                );
        return -abs(score);
}

template<bool Color, typename Board>
int Evaluate::mobility(const Position<Board>& p)
{
        return Weight::MOBILITY * generate::Successors<rules::International, Board>::count_mobility<Color>(p);
}

template<typename Board>
void Evaluate::print_break_down(const Position<Board>& p)
{
        std::cout << "Term    " << " BLACK"                                 << " WHITE"                                 << " DELTA"                                       << std::endl;
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout << std::endl;
        std::cout << "Material" << std::setw(6) << material<Side::BLACK>(p) << std::setw(6) << material<Side::WHITE>(p) << std::setw(6) << delta_material<Side::BLACK>(p) << std::endl;
        std::cout << "Tempo   " << std::setw(6) << tempo<Side::BLACK>(p)    << std::setw(6) << tempo<Side::WHITE>(p)    << std::setw(6) << delta_tempo<Side::BLACK>(p)    << std::endl;
        std::cout << "Center  " << std::setw(6) << center<Side::BLACK>(p)   << std::setw(6) << center<Side::WHITE>(p)   << std::setw(6) << delta_center<Side::BLACK>(p)   << std::endl;
        std::cout << "Balance " << std::setw(6) << balance<Side::BLACK>(p)  << std::setw(6) << balance<Side::WHITE>(p)  << std::setw(6) << delta_balance<Side::BLACK>(p)  << std::endl;
        std::cout << "Mobility" << std::setw(6) << mobility<Side::BLACK>(p) << std::setw(6) << mobility<Side::WHITE>(p) << std::setw(6) << delta_mobility<Side::BLACK>(p) << std::endl;
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout << std::endl;
        std::cout << "Evaluate" << std::setw(6) << evaluate<Side::BLACK>(p) << std::setw(6) << evaluate<Side::WHITE>(p) << std::setw(6) << delta_evaluate<Side::BLACK>(p) << std::endl;
        std::cout << std::endl;
}

}       // namespace dctl