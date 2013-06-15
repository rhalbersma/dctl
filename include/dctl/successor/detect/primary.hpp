#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool /* Color */, class /* Pieces */, class /* Select */, class /* Range */>
struct detect
{
        template<class Position>
        bool operator()(Position const& /* p */) const
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
