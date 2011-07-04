#include "Parameters.h"

tree::search::Parameters::Parameters(void)
{
}

const tree::node::Sequence& tree::search::Parameters::PV(void) const
{
        return PV_;
}

tree::node::Sequence& tree::search::Parameters::PV(void)
{
        return PV_;
}

size_t tree::search::Parameters::best_move(void) const
{
        return *PV().begin();
}

void tree::search::Parameters::set_PV(size_t first_move, const node::Sequence& continuation)
{
        PV().resize(1 + continuation.size());
        *PV().begin() = first_move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

void tree::search::Parameters::clear_PV(void)
{
        PV().clear();
}
