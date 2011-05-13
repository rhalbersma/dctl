#include "Root.h"
#include <iomanip>
#include <iostream>

Tree::Walk::Root::TranspositionTable Tree::Walk::Root::TT(1);
Tree::Statistics Tree::Walk::Root::statistics_;

void Tree::Walk::Root::report(size_t nominal_ply, NodeCount leafs, const Timer& timer)
{
        double speed = (statistics_.nodes() / 1e6) / timer.lap();
        double average_ply = static_cast<double>(statistics_.sum_ply()) / statistics_.nodes();

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
        std::cout << std::setw(11) << statistics_.nodes();
        std::cout << " nodes, ";
        std::cout << std::setw( 6) << timer.elapsed();
        std::cout << "s, ";
        std::cout << std::setw( 5) << speed;
        std::cout << " Mnps";
        std::cout << std::endl;
}

void Tree::Walk::Root::summary(void)
{
        std::cout << std::endl;
}

void Tree::Walk::Root::summary(NodeCount leafs)
{
        std::cout << std::endl << "Total leafs: " << leafs << std::endl;
        std::cout << std::endl;
}

void Tree::Walk::Root::print_move(const std::string& move, size_t i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}
