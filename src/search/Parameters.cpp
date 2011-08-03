#include "Parameters.h"
#include <algorithm>    // std::copy

namespace dctl {
namespace search {

Parameters::Parameters()
{
        PV_.reserve(32);
}

const Sequence& Parameters::PV() const
{
        return PV_;
}

Sequence& Parameters::PV()
{
        return PV_;
}

int Parameters::best_move() const
{
        return *PV().begin();
}

void Parameters::set_PV(int first_move, const Sequence& continuation)
{
        *PV().begin() = first_move;
        std::copy(continuation.begin(), continuation.end(), PV().begin() + 1);
}

}       // namespace search
}       // namespace dctl
