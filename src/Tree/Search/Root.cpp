#include <algorithm>    // std::generate
#include <iomanip>
#include <iostream>
#include "Root.h"
#include "../../utils/UniqueNumber.h"

namespace tree {
namespace search {

Root::TranspositionTable Root::TT(0);
Statistics Root::statistics_;
bool Root::interrupted_ = false;

// UCI format
void Root::report(int depth, int value, const Timer& timer)
{
        std::cout << "info";

        std::cout << " score ";
        std::cout << std::setw( 4) << std::right << score::print(value);

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

        const double hashfull = 1000 * (static_cast<double>((TT.size() - TT.available())) / TT.size());
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;

        std::cout << std::endl;
}

/*
// Winboard format
void Root::report(int depth, int value, const Timer& timer)
{
        std::cout << std::setw( 2) << std::right << depth;

        std::cout << std::setw( 4) << std::right << score::print(value);

        std::cout << std::setw(12) << std::right << statistics_.nodes();

        std::cout << std::setw( 6) << std::right << timer.elapsed();

        std::cout << std::endl;
}
*/

void Root::identity_permutation(node::Order& permutation)
{
        UniqueNumber unique_number;
        std::generate(permutation.begin(), permutation.end(), unique_number);
}

bool Root::is_PV(int node)
{
        return node == PV;
}

void Root::resize_hash(size_t s)
{
        return TT.resize(s);
}

void Root::clear_hash()
{
        return TT.clear();
}

void Root::interrupt()
{
        interrupted_ = true;
}

bool Root::is_interrupted()
{
        return interrupted_;
}

}       // namespace search
}       // namespace tree
