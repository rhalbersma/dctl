#include "EvaluateWeight.h"
#include "../Tree/Generate/Successors.h"
#include "../Tree/Node/Position.h"
#include "../Tree/Node/Side.h"
#include "../Variants/Rules.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

template<typename Board>
int Evaluate::evaluate(const Tree::Node::Position<Board>& p)
{
        return p.to_move()? delta_evaluate<Tree::Node::Side::WHITE>(p) : delta_evaluate<Tree::Node::Side::BLACK>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_evaluate(const Tree::Node::Position<Board>& p)
{
        return evaluate<Color>(p) - evaluate<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_material(const Tree::Node::Position<Board>& p)
{
        return material<Color>(p) - material<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_tempo(const Tree::Node::Position<Board>& p)
{
        return tempo<Color>(p) - tempo<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_center(const Tree::Node::Position<Board>& p)
{
        return center<Color>(p) - center<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_balance(const Tree::Node::Position<Board>& p)
{
        return balance<Color>(p) - balance<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::delta_mobility(const Tree::Node::Position<Board>& p)
{
        return mobility<Color>(p) - mobility<!Color>(p);
}

template<bool Color, typename Board>
int Evaluate::evaluate(const Tree::Node::Position<Board>& p)
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
int Evaluate::material(const Tree::Node::Position<Board>& p)
{
        return
        (
                EvaluateWeight::MATERIAL[0] * Bit::count(p.pieces(Color)) +
                EvaluateWeight::MATERIAL[1] * Bit::count(p.kings(Color))
        );
}

template<bool Color, typename Board>
int Evaluate::tempo(const Tree::Node::Position<Board>& p)
{
        size_t score = 0;
        for (size_t i = 1; i < Board::HEIGHT; ++i)
                score += EvaluateWeight::TEMPO[i] * Bit::count(p.pieces(Color) & Board::ROW_MASK[Color][i]);
        return score;
}

template<bool Color, typename Board>
int Evaluate::center(const Tree::Node::Position<Board>& p)
{
        size_t score = 0;
        for (size_t i = 1; i < Board::WIDTH / 2; ++i)
                score += EvaluateWeight::CENTER[i] *
                (
                        Bit::count(p.pieces(Color) & Board::COL_MASK[ Color][i]) +
                        Bit::count(p.pieces(Color) & Board::COL_MASK[!Color][i])
                );
        return score;
}

template<bool Color, typename Board>
int Evaluate::balance(const Tree::Node::Position<Board>& p)
{
        int score = 0;
        for (size_t i = 0; i < Board::WIDTH / 2; ++i)
                score += EvaluateWeight::BALANCE[i] *
                (
                        Bit::count(p.pieces(Color) & Board::COL_MASK[ Color][i]) -
                        Bit::count(p.pieces(Color) & Board::COL_MASK[!Color][i])
                );
        return -abs(score);
}

template<bool Color, typename Board>
int Evaluate::mobility(const Tree::Node::Position<Board>& p)
{
        return EvaluateWeight::MOBILITY * Tree::Generate::Successors<Variants::International, Board>::count_mobility<Color>(p);
}

template<typename Board>
void Evaluate::print_break_down(const Tree::Node::Position<Board>& p)
{
        std::cout << "Term    " << " BLACK"                                 << " WHITE"                                 << " DELTA"                                       << std::endl;
        for (size_t i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout << std::endl;
        std::cout << "Material" << std::setw(6) << material<Tree::Node::Side::BLACK>(p) << std::setw(6) << material<Tree::Node::Side::WHITE>(p) << std::setw(6) << delta_material<Tree::Node::Side::BLACK>(p) << std::endl;
        std::cout << "Tempo   " << std::setw(6) << tempo<Tree::Node::Side::BLACK>(p)    << std::setw(6) << tempo<Tree::Node::Side::WHITE>(p)    << std::setw(6) << delta_tempo<Tree::Node::Side::BLACK>(p)    << std::endl;
        std::cout << "Center  " << std::setw(6) << center<Tree::Node::Side::BLACK>(p)   << std::setw(6) << center<Tree::Node::Side::WHITE>(p)   << std::setw(6) << delta_center<Tree::Node::Side::BLACK>(p)   << std::endl;
        std::cout << "Balance " << std::setw(6) << balance<Tree::Node::Side::BLACK>(p)  << std::setw(6) << balance<Tree::Node::Side::WHITE>(p)  << std::setw(6) << delta_balance<Tree::Node::Side::BLACK>(p)  << std::endl;
        std::cout << "Mobility" << std::setw(6) << mobility<Tree::Node::Side::BLACK>(p) << std::setw(6) << mobility<Tree::Node::Side::WHITE>(p) << std::setw(6) << delta_mobility<Tree::Node::Side::BLACK>(p) << std::endl;
        for (size_t i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout << std::endl;
        std::cout << "Evaluate" << std::setw(6) << evaluate<Tree::Node::Side::BLACK>(p) << std::setw(6) << evaluate<Tree::Node::Side::WHITE>(p) << std::setw(6) << delta_evaluate<Tree::Node::Side::BLACK>(p) << std::endl;
        std::cout << std::endl;
}
