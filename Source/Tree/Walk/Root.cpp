#include <iomanip>
#include <iostream>
#include "Root.h"

tree::walk::Root::TranspositionTable tree::walk::Root::TT(1);
tree::Statistics tree::walk::Root::statistics_;

void tree::walk::Root::report(NodeCount leafs, int depth, const Timer& timer)
{
        std::cout << "info";

        std::cout << " leafs ";
        std::cout << std::setw( 4) << std::left << leafs;

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " nodes ";
        std::cout << std::setw(12) << std::right << statistics_.nodes();

        std::cout << " time ";
        std::cout << std::setw( 6) << timer.elapsed();

        const double nps = (1000 * statistics_.nodes()) / static_cast<double>(timer.lap());
        std::cout << " nps ";
        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
        std::cout << std::setw( 7) << nps;

        const double hashfull = (1000 * (TT.size() - TT.empty())) / static_cast<double>(TT.size());
        std::cout << " hashfull ";
        std::cout << std::setw( 3) << std::right << hashfull;

        std::cout << std::endl;
}

void tree::walk::Root::summary(void)
{
        std::cout << std::endl;
}

void tree::walk::Root::summary(NodeCount leafs)
{
        std::cout << std::endl << "Total leafs: " << leafs << std::endl;
        std::cout << std::endl;
}

void tree::walk::Root::print_move(const std::string& move, int i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

void tree::walk::Root::clear_hash(void)
{
        return TT.clear();
}
