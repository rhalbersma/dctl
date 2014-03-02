#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>

namespace dctl {
namespace successor {

template<bool /* Color */, class /* Pieces */, class /* Select */, class /* Range */>
struct Detect
{
        template<class Position>
        bool operator()(Position const& /* p */) const
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
