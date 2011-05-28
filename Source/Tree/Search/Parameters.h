#pragma once
#include "../Move/Types.h"

namespace Node { template<typename> class Position; }

namespace Tree {
namespace Search {

class Parameters
{
public:
        // constructors
        Parameters(void);

        // views
        const Tree::Move::Sequence& PV(void) const;
        Move::Sequence& PV(void);
        size_t best_move(void) const;

        // modifiers
        void set_PV(size_t, const Move::Sequence&);
        void clear_PV(void);

private:
        // implementation
        static const bool TOGGLE = true;

        // representation
        Move::Sequence PV_;
};

}       // namespace Search
}       // namespace Tree
