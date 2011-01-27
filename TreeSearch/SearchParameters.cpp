#include "SearchParameters.h"

SearchParameters::SearchParameters(void)
:
        null_move_(true)
{
}

const Move::Sequence& SearchParameters::PV(void) const
{
        return PV_;
}

Move::Sequence& SearchParameters::PV(void)
{
        return PV_;
}

size_t SearchParameters::best_move(void) const
{
        return *PV().begin();
}

bool SearchParameters::do_null_move(void) const
{
        return null_move_;
}

void SearchParameters::reset_PV(size_t move, const Move::Sequence& continuation)
{
        PV().resize(1 + continuation.size());
        *PV().begin() = move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

void SearchParameters::clear_do_null_move()
{
        null_move_ = false;
}

void SearchParameters::set_do_null_move()
{
        null_move_ = true;
}
