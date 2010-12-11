#include "SearchParameters.h"

SearchParameters::SearchParameters(void)
:
        d_null_move(true)
{
}

const Move::Sequence& SearchParameters::PV(void) const
{
        return d_PV;
}

Move::Sequence& SearchParameters::PV(void)
{
        return d_PV;
}

size_t SearchParameters::best_move(void) const
{
        return *PV().begin();
}

bool SearchParameters::do_null_move(void) const
{
        return d_null_move;
}

void SearchParameters::reset_PV(size_t move, const Move::Sequence& continuation)
{
        PV().resize(1 + continuation.size());
        *PV().begin() = move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

void SearchParameters::clear_do_null_move()
{
        d_null_move = false;
}

void SearchParameters::set_do_null_move()
{
        d_null_move = true;
}
