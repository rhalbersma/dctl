#include "Parameters.h"

namespace tree {
namespace search {

Parameters::Parameters()
{
}

const tree::node::Sequence& Parameters::PV() const
{
        return PV_;
}

tree::node::Sequence& Parameters::PV()
{
        return PV_;
}

size_t Parameters::best_move() const
{
        return *PV().begin();
}

void Parameters::set_PV(size_t first_move, const node::Sequence& continuation)
{
        PV().resize(1 + continuation.size());
        *PV().begin() = first_move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

void Parameters::clear_PV()
{
        PV().clear();
}

}       // namespace search
}       // namespace tree
