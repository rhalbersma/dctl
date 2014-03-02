#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)

namespace dctl {
namespace successor {

template<bool /* Color */, class /* Pieces */, class /* Select */>
struct Count
{
        template<class Position>
        int operator()(Position const& /* p */) const
        {
                return 0;
        }
};

}       // namespace successor
}       // namespace dctl
