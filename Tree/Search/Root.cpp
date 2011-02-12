#include "Root.h"
#include "../../Utilities/UniqueNumber.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

Tree::Search::Root::TranspositionTable Tree::Search::Root::TT(10);
Tree::Statistics Tree::Search::Root::statistics_;

void Tree::Search::Root::report(size_t nominal_ply, int alpha, int beta, int value, const StopWatch& timer)
{
        double speed = (statistics_.nodes() / 1e6) / timer.delta();
        double average_ply = static_cast<double>(statistics_.sum_ply()) / statistics_.nodes();

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(1);
        std::cout << "search";
        std::cout << "[";
        std::cout << std::setw(2) << nominal_ply;
        std::cout << "/";
        std::cout << std::setw(4) << average_ply;
        std::cout << "/";
        std::cout << std::setw(2) << statistics_.max_ply();
        std::cout << "; ";
        std::cout << std::setw(4) << Value::print(alpha);
        std::cout << ", ";
        std::cout << std::setw(4) << Value::print(beta);
        std::cout << "] = ";

        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(2);
        std::cout << std::setw( 4) << Value::print(value);
        std::cout << ", ";
        std::cout << std::setw(12) << statistics_.nodes();
        std::cout << " nodes, ";
        std::cout << std::setw( 6) << timer.total();
        std::cout << "s, ";
        std::cout << std::setw( 4) << speed;
        std::cout << " Mnps";
        std::cout << std::endl;
}

void Tree::Search::Root::identity_permutation(Move::Order& permutation)
{
        UniqueNumber unique_number;
        std::generate(permutation.begin(), permutation.end(), unique_number);
}

bool Tree::Search::Root::is_PV(size_t node)
{
        return node == PV;
}
