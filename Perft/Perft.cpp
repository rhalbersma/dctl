#include "Perft.h"
#include <iostream>
#include <iomanip>

Perft::TranspositionTable Perft::TT;
SearchStatistics Perft::statistics_;

void Perft::report(size_t nominal_ply, NodeCount leafs, const StopWatch& timer)
{
        double speed = (nodes() / 1e6) / timer.delta();
        double average_ply = static_cast<double>(sum_ply()) / nodes();

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(1);
        std::cout << "perft";
        std::cout << "[";
        std::cout << std::setw(2) << nominal_ply;
        std::cout << "/";                
        std::cout << std::setw(4) << average_ply;
        std::cout << "] = ";

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        std::cout << std::setw(12) << leafs;
        std::cout << " leafs, ";
        std::cout << std::setw(11) << nodes();
        std::cout << " nodes, ";
        std::cout << std::setw( 6) << timer.delta();
        std::cout << "s, ";
        std::cout << std::setw( 5) << speed;
        std::cout << " Mnps";
        std::cout << std::endl;
}

NodeCount Perft::nodes(void)
{
        return statistics_.nodes();
}

NodeCount Perft::sum_ply(void)
{
        return statistics_.sum_ply();
}

size_t Perft::max_ply(void)
{
        return statistics_.max_ply();
}

void Perft::reset_statistics(void)
{
        statistics_.reset();
}

void Perft::update_statistics(size_t ply)
{
        statistics_.update(ply);
}
