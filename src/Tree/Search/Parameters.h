#pragma once
#include "../Node/Stack.h"

namespace node { template<typename> class Position; }

namespace tree {
namespace search {

class Parameters
{
public:
        // constructors
        Parameters(void);

        // views
        const tree::node::Sequence& PV(void) const;
        node::Sequence& PV(void);
        size_t best_move(void) const;

        // modifiers
        void set_PV(size_t, const node::Sequence&);
        void clear_PV(void);

private:
        // representation
        node::Sequence PV_;
};

}       // namespace search
}       // namespace tree
