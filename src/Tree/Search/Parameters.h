#pragma once
#include "../Node/Stack.h"

namespace node { template<typename> class Position; }

namespace tree {
namespace search {

class Parameters
{
public:
        // constructors
        Parameters();

        // views
        const tree::node::Sequence& PV() const;
        node::Sequence& PV();
        size_t best_move() const;

        // modifiers
        void set_PV(size_t, const node::Sequence&);
        void clear_PV();

private:
        // representation
        node::Sequence PV_;
};

}       // namespace search
}       // namespace tree
