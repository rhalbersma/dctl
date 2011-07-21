#pragma once
#include "../node/Stack.h"

namespace tree {
namespace search {

class Parameters
{
public:
        // constructors
        Parameters();

        // views
        const Sequence& PV() const;
        Sequence& PV();
        size_t best_move() const;

        // modifiers
        void set_PV(size_t, const Sequence&);
        void clear_PV();

private:
        // representation
        Sequence PV_;
};

}       // namespace search
}       // namespace tree
