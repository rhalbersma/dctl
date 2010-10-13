#include "Perft.h"
#include <iostream>
#include <iomanip>

Perft::TranspositionTable Perft::TT;
SearchStatistics Perft::d_statistics;

void Perft::report(NodeCount leafs, size_t nominal_depth, double time_used, bool print_average_ply)
{
        double speed = (nodes() / 1e6) / time_used;
        double average_ply = static_cast<double>(sum_ply()) / nodes();

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(1);
        std::cout << "perft";
        std::cout << "[";
        std::cout << std::setw(2) << nominal_depth;
        if (print_average_ply) {
                std::cout << "/";
                std::cout << std::setw(4) << average_ply;
        }
        std::cout << "] = ";

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        std::cout << std::setw(12) << leafs;
        std::cout << " leafs, ";
        std::cout << std::setw(12) << nodes();
        std::cout << " nodes, ";
        std::cout << std::setw( 6) << time_used;
        std::cout << "s, ";
        std::cout << std::setw( 6) << speed;
        std::cout << " Mnps";
        std::cout << std::endl;
}

NodeCount Perft::nodes(void)
{
        return d_statistics.nodes();
}

NodeCount Perft::sum_ply(void)
{
        return d_statistics.sum_ply();
}

size_t Perft::max_ply(void)
{
        return d_statistics.max_ply();
}

void Perft::reset_statistics(void)
{
        d_statistics.reset();
}

void Perft::update_statistics(void)
{
        d_statistics.update();
}

void Perft::update_statistics(size_t ply)
{
        d_statistics.update(ply);
}
