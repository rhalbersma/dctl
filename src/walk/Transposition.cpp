#include "Transposition.h"

namespace dctl {
namespace walk {

Transposition::Transposition()
:
        entry_(0)
{
}

Transposition::Transposition(NodeCount n, int d)
:
        entry_(0)
{
        entry_ ^= (n & LEAFS_MASK) << LEAFS_SHIFT;
        entry_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
}

NodeCount Transposition::leafs() const
{
        return (entry_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

int Transposition::depth() const
{
        return static_cast<int>((entry_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}

}       // namespace walk
}       // namespace dctl
