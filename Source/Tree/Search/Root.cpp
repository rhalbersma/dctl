#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Root.h"
#include "../../Utilities/UniqueNumber.h"

tree::search::Root::TranspositionTable tree::search::Root::TT(27);
tree::Statistics tree::search::Root::statistics_;

void tree::search::Root::report(int score, int depth, const Timer& timer)
{
        std::cout << "info";

        std::cout << " score ";
        std::cout << std::setw( 4) << std::left << value::print(score);

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

void tree::search::Root::identity_permutation(move::Order& permutation)
{
        UniqueNumber unique_number;
        std::generate(permutation.begin(), permutation.end(), unique_number);
}

bool tree::search::Root::is_PV(int node)
{
        return node == PV;
}

void tree::search::Root::clear_hash(void)
{
        return TT.clear();
}
