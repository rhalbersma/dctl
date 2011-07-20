#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "EvaluateWeight.h"
#include "../Tree/Generate/Successors.h"
#include "../Tree/Node/Position.h"
#include "../Tree/Node/Side.h"
#include "../rules/Variants.h"

template<typename Board>
int Evaluate::evaluate(const tree::node::Position<Board>& p)
{
        return p.to_move()? delta_evaluate<tree::node::Side::WHITE>(p) : delta_evaluate<tree::node::Side::BLACK>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_evaluate(const tree::node::Position<Board>& p)
{
        return evaluate<Color>(p) - evaluate<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_material(const tree::node::Position<Board>& p)
{
        return material<Color>(p) - material<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_tempo(const tree::node::Position<Board>& p)
{
        return tempo<Color>(p) - tempo<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_center(const tree::node::Position<Board>& p)
{
        return center<Color>(p) - center<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_balance(const tree::node::Position<Board>& p)
{
        return balance<Color>(p) - balance<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_mobility(const tree::node::Position<Board>& p)
{
        return mobility<Color>(p) - mobility<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::evaluate(const tree::node::Position<Board>& p)
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
int Evaluate::material(const tree::node::Position<Board>& p)
{
        return
        (
                EvaluateWeight::MATERIAL[0] * bit::count(p.pieces(Color)) +
                EvaluateWeight::MATERIAL[1] * bit::count(p.kings(Color))
        );
}

template<bool Color, typename Board>
int Evaluate::tempo(const tree::node::Position<Board>& p)
{
        int score = 0;
        for (auto i = 1; i < Board::HEIGHT; ++i)
                score += EvaluateWeight::TEMPO[i] * bit::count(p.pieces(Color) & Board::ROW_MASK[Color][i]);
        return score;
}

template<bool Color, typename Board>
int Evaluate::center(const tree::node::Position<Board>& p)
{
        int score = 0;
        for (auto i = 1; i < Board::WIDTH / 2; ++i)
                score += EvaluateWeight::CENTER[i] *
                (
                        bit::count(p.pieces(Color) & Board::COL_MASK[ Color][i]) +
                        bit::count(p.pieces(Color) & Board::COL_MASK[!Color][i])
                );
        return score;
}

template<bool Color, typename Board>
int Evaluate::balance(const tree::node::Position<Board>& p)
{
        int score = 0;
        for (auto i = 0; i < Board::WIDTH / 2; ++i)
                score += EvaluateWeight::BALANCE[i] *
                (
                        bit::count(p.pieces(Color) & Board::COL_MASK[ Color][i]) -
                        bit::count(p.pieces(Color) & Board::COL_MASK[!Color][i])
                );
        return -abs(score);
}

template<bool Color, typename Board>
int Evaluate::mobility(const tree::node::Position<Board>& p)
{
        return EvaluateWeight::MOBILITY * tree::generate::Successors<rules::International, Board>::count_mobility<Color>(p);
}

template<typename Board>
void Evaluate::print_break_down(const tree::node::Position<Board>& p)
{
        std::cout << "Term    " << " BLACK"                                 << " WHITE"                                 << " DELTA"                                       << std::endl;
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout << std::endl;
        std::cout << "Material" << std::setw(6) << material<tree::node::Side::BLACK>(p) << std::setw(6) << material<tree::node::Side::WHITE>(p) << std::setw(6) << delta_material<tree::node::Side::BLACK>(p) << std::endl;
        std::cout << "Tempo   " << std::setw(6) << tempo<tree::node::Side::BLACK>(p)    << std::setw(6) << tempo<tree::node::Side::WHITE>(p)    << std::setw(6) << delta_tempo<tree::node::Side::BLACK>(p)    << std::endl;
        std::cout << "Center  " << std::setw(6) << center<tree::node::Side::BLACK>(p)   << std::setw(6) << center<tree::node::Side::WHITE>(p)   << std::setw(6) << delta_center<tree::node::Side::BLACK>(p)   << std::endl;
        std::cout << "Balance " << std::setw(6) << balance<tree::node::Side::BLACK>(p)  << std::setw(6) << balance<tree::node::Side::WHITE>(p)  << std::setw(6) << delta_balance<tree::node::Side::BLACK>(p)  << std::endl;
        std::cout << "Mobility" << std::setw(6) << mobility<tree::node::Side::BLACK>(p) << std::setw(6) << mobility<tree::node::Side::WHITE>(p) << std::setw(6) << delta_mobility<tree::node::Side::BLACK>(p) << std::endl;
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout << std::endl;
        std::cout << "Evaluate" << std::setw(6) << evaluate<tree::node::Side::BLACK>(p) << std::setw(6) << evaluate<tree::node::Side::WHITE>(p) << std::setw(6) << delta_evaluate<tree::node::Side::BLACK>(p) << std::endl;
        std::cout << std::endl;
}
