#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // generate (primary template)

namespace dctl {
namespace successor {
namespace detail {

// primary template (default implementation)
template<bool /* Color */, class /* Pieces */, class /* Select */>
struct generate
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
