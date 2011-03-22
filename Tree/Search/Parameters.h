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
        bool do_null_move(void) const;

        // modifiers
        void reset_PV(size_t, const Move::Sequence&);
        void clear_do_null_move(void);
        void set_do_null_move(void);

private:
        // implementation
        static const bool TOGGLE = true;

        // representation
        Move::Sequence PV_;
        bool null_move_;
};

}       // namespace Search
}       // namespace Tree
