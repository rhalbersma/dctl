#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Root.h"
#include "../../Utilities/UniqueNumber.h"

tree::search::Root::TranspositionTable tree::search::Root::TT(10);
tree::Statistics tree::search::Root::statistics_;

void tree::search::Root::report(int nominal_ply, int value, const Timer& timer)
{
        double speed = (statistics_.nodes() / 1e6) / timer.lap();
        double average_ply = static_cast<double>(statistics_.sum_ply()) / statistics_.nodes();

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(1);
        std::cout << "depth ";
        std::cout << std::setw(2) << nominal_ply;
        std::cout << "/";
        std::cout << std::setw(4) << average_ply;
        std::cout << "/";
        std::cout << std::setw(2) << statistics_.max_ply();

        std::cout << " value = ";
        std::cout << std::setw( 4) << std::left << value::print(value);
        std::cout << " ";

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        std::cout << std::setw(12) << std::right << statistics_.nodes();
        std::cout << " nodes, ";
        std::cout << std::setw( 6) << timer.elapsed();
        std::cout << "s, ";
        std::cout << std::setw( 4) << speed;
        std::cout << " Mnps";
        std::cout << std::endl;
}

void tree::search::Root::identity_permutation(move::Order& permutation)
{
        UniqueNumber unique_number;
        std::generate(permutation.begin(), permutation.end(), unique_number);
}

bool tree::search::Root::is_PV(int node)
{
        return node == PV;
}
