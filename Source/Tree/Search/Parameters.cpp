#include "Parameters.h"

Tree::Search::Parameters::Parameters(void)
{
}

const Tree::Move::Sequence& Tree::Search::Parameters::PV(void) const
{
        return PV_;
}

Tree::Move::Sequence& Tree::Search::Parameters::PV(void)
{
        return PV_;
}

size_t Tree::Search::Parameters::best_move(void) const
{
        return *PV().begin();
}

void Tree::Search::Parameters::set_PV(size_t first_move, const Move::Sequence& continuation)
{
        PV().resize(1 + continuation.size());
        *PV().begin() = first_move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

void Tree::Search::Parameters::clear_PV(void)
{
        PV().clear();
}
