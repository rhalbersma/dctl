#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "Weight.h"
#include "../successor/Mobility.hpp"
#include "../node/Position.h"
#include "../node/Side.hpp"
#include "../variant/International.hpp"

namespace dctl {

template<typename Board>
int Evaluate::evaluate(const Position<Board>& p)
{
        return (p.active_color() == Side::white)? delta_evaluate<Side::white>(p) : delta_evaluate<Side::black>(p);
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
        return (
                Weight::MATERIAL[0] * bit::count(p.pieces(Color)) +
                Weight::MATERIAL[1] * bit::count(p.kings(Color))
        );
}

template<bool Color, typename Board>
int Evaluate::tempo(const Position<Board>& p)
{
        int score = 0;
        for (auto i = 1; i < Board::height; ++i)
                score += Weight::TEMPO[i] * bit::count(p.pieces(Color) & Board::row_mask[Color][i]);
        return score;
}

template<bool Color, typename Board>
int Evaluate::center(const Position<Board>& p)
{
        int score = 0;
        for (auto i = 1; i < Board::width / 2; ++i)
                score += Weight::CENTER[i] *
                (
                        bit::count(p.pieces(Color) & Board::col_mask[ Color][i]) +
                        bit::count(p.pieces(Color) & Board::col_mask[!Color][i])
                );
        return score;
}

template<bool Color, typename Board>
int Evaluate::balance(const Position<Board>& p)
{
        int score = 0;
        for (auto i = 0; i < Board::width / 2; ++i)
                score += Weight::BALANCE[i] *
                (
                        bit::count(p.pieces(Color) & Board::col_mask[ Color][i]) -
                        bit::count(p.pieces(Color) & Board::col_mask[!Color][i])
                );
        return -abs(score);
}

template<bool Color, typename Board>
int Evaluate::mobility(const Position<Board>& p)
{
        return Weight::MOBILITY * Mobility<successor::Moves, variant::International>::template count<Color>(p);
}

template<typename Board>
void Evaluate::print_break_down(const Position<Board>& p)
{
        std::cout << "Term    " << " BLACK"                                 << " WHITE"                                 << " DELTA"                                       << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Material" << std::setw(6) << material<Side::black>(p) << std::setw(6) << material<Side::white>(p) << std::setw(6) << delta_material<Side::black>(p) << "\n";
        std::cout << "Tempo   " << std::setw(6) << tempo<Side::black>(p)    << std::setw(6) << tempo<Side::white>(p)    << std::setw(6) << delta_tempo<Side::black>(p)    << "\n";
        std::cout << "Center  " << std::setw(6) << center<Side::black>(p)   << std::setw(6) << center<Side::white>(p)   << std::setw(6) << delta_center<Side::black>(p)   << "\n";
        std::cout << "Balance " << std::setw(6) << balance<Side::black>(p)  << std::setw(6) << balance<Side::white>(p)  << std::setw(6) << delta_balance<Side::black>(p)  << "\n";
        std::cout << "Mobility" << std::setw(6) << mobility<Side::black>(p) << std::setw(6) << mobility<Side::white>(p) << std::setw(6) << delta_mobility<Side::black>(p) << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Evaluate" << std::setw(6) << evaluate<Side::black>(p) << std::setw(6) << evaluate<Side::white>(p) << std::setw(6) << delta_evaluate<Side::black>(p) << "\n";
        std::cout<< "\n";
}

}       // namespace dctl
