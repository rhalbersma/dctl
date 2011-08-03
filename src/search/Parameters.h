#pragma once
#include "../node/Move.h"

namespace dctl {
namespace search {

class Parameters
{
public:
        // constructors
        Parameters();

        // views
        const Sequence& PV() const;
        Sequence& PV();
        int best_move() const;

        // modifiers
        void set_PV(int, const Sequence&);

private:
        // representation
        Sequence PV_;
};

}       // namespace search
}       // namespace dctl
