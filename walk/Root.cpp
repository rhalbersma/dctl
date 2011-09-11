#include <iomanip>
#include <iostream>
#include "Root.h"
#include "../utility/Timer.h"

namespace dctl {
namespace walk {

Root::TranspositionTable Root::TT(1);
Statistics Root::statistics_;

void Root::report(int depth, NodeCount leafs, const Timer& timer)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

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

        std::cout << "\n";
}

void Root::summary()
{
        std::cout << "\n";
}

void Root::summary(NodeCount leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

void Root::print_move(const std::string& move, int i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

void Root::resize_hash(std::size_t s)
{
        return TT.resize(s);
}

void Root::clear_hash()
{
        return TT.clear();
}

}       // namespace walk
}       // namespace dctl
