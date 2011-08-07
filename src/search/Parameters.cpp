#include "Parameters.h"
#include <algorithm>    // std::copy
#include <iterator>

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

int Parameters::best_move() const
{
        return *PV_.begin();
}

void Parameters::set_PV(int first_move, const Sequence& continuation)
{
        PV_.clear();
        PV_.push_back(first_move);
        PV_.insert(PV_.end(), continuation.begin(), continuation.end());
}

}       // namespace search
}       // namespace dctl
