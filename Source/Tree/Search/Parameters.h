#pragma once
#include "../Move/Stack.h"

namespace node { template<typename> class Position; }

namespace tree {
namespace search {

class Parameters
{
public:
        // constructors
        Parameters(void);

        // views
        const tree::move::Sequence& PV(void) const;
        move::Sequence& PV(void);
        size_t best_move(void) const;

        // modifiers
        void set_PV(size_t, const move::Sequence&);
        void clear_PV(void);

private:
        // representation
        move::Sequence PV_;
};

}       // namespace search
}       // namespace tree
