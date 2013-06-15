#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)

namespace dctl {
namespace successor {
namespace detail {

// primary template (default implementation)
template<bool /* Color */, class /* Pieces */, class /* Select */>
struct copy
{
        template<class Position, class Sequence>
        void operator()(Position const& /* p */, Sequence& /* moves */) const
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
