#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace directions {

struct up
:
        boost::mpl::identity<up>
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
