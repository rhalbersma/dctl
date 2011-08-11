#include "Root.h"

namespace dctl {
namespace search {

Root::TranspositionTable Root::TT(0);
Statistics Root::statistics_;
bool Root::interrupted_ = false;

/*
// Winboard format
void Root::report(int depth, int value, const Timer& timer)
{
        std::cout << std::setw( 2) << std::right << depth;

        std::cout << std::setw( 4) << std::right << print(value);

        std::cout << std::setw(12) << std::right << statistics_.nodes();

        std::cout << std::setw( 6) << std::right << timer.elapsed();

        std::cout << std::endl;
}
*/

bool Root::is_PV(int node)
{
        return node == PV;
}

void Root::resize_hash(std::size_t s)
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
}       // namespace dctl
