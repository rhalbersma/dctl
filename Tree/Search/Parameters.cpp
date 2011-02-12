#include "Parameters.h"

Tree::Search::Parameters::Parameters(void)
:
        null_move_(true)
{
}

const Move::Sequence& Tree::Search::Parameters::PV(void) const
{
        return PV_;
}

Move::Sequence& Tree::Search::Parameters::PV(void)
{
        return PV_;
}

size_t Tree::Search::Parameters::best_move(void) const
{
        return *PV().begin();
}

bool Tree::Search::Parameters::do_null_move(void) const
{
        return null_move_;
}

void Tree::Search::Parameters::reset_PV(size_t move, const Move::Sequence& continuation)
{
        PV().resize(1 + continuation.size());
        *PV().begin() = move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

void Tree::Search::Parameters::clear_do_null_move()
{
        null_move_ = false;
}

void Tree::Search::Parameters::set_do_null_move()
{
        null_move_ = true;
}
