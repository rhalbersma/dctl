#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)

namespace dctl {
namespace successor {

// primary template (default implementation)
template<bool /* Color */, class /* Pieces */, class /* Select */>
struct Generate
{
        template<class Position, class Sequence>
        void operator()(Position const& /* p */, Sequence& /* moves */) const
        {
                // no-op
        }
};

}       // namespace successor
}       // namespace dctl
