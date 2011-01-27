#include "Search.h"
#include "../Utilities/UniqueNumber.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

TreeSearch::Search::TranspositionTable TreeSearch::Search::TT(10);
SearchStatistics TreeSearch::Search::statistics_;

void TreeSearch::Search::report(size_t nominal_ply, int alpha, int beta, int value, const StopWatch& timer)
{
        double speed = (nodes() / 1e6) / timer.delta();
        double average_ply = static_cast<double>(sum_ply()) / nodes();

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(1);
        std::cout << "search";
        std::cout << "[";
        std::cout << std::setw(2) << nominal_ply;
        std::cout << "/";
        std::cout << std::setw(4) << average_ply;
        std::cout << "/";
        std::cout << std::setw(2) << max_ply();
        std::cout << "; ";
        std::cout << std::setw(4) << Value::print(alpha);
        std::cout << ", ";
        std::cout << std::setw(4) << Value::print(beta);
        std::cout << "] = ";

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        std::cout << std::setw( 4) << Value::print(value);
        std::cout << ", ";
        std::cout << std::setw(12) << nodes();
        std::cout << " nodes, ";
        std::cout << std::setw( 6) << timer.total();
        std::cout << "s, ";
        std::cout << std::setw( 4) << speed;
        std::cout << " Mnps";
        std::cout << std::endl;
}

NodeCount TreeSearch::Search::nodes(void)
{
        return statistics_.nodes();
}

NodeCount TreeSearch::Search::sum_ply(void)
{
        return statistics_.sum_ply();
}

size_t TreeSearch::Search::max_ply(void)
{
        return statistics_.max_ply();
}

void TreeSearch::Search::reset_statistics(void)
{
        statistics_.reset();
}

void TreeSearch::Search::update_statistics(size_t ply)
{
        statistics_.update(ply);
}

void TreeSearch::Search::identity_permutation(Move::Order& permutation)
{
        UniqueNumber unique_number;
        std::generate(permutation.begin(), permutation.end(), unique_number);
}

bool TreeSearch::Search::is_PV(size_t node)
{
        return node == PV;
}
