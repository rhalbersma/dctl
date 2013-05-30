#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace range {

struct distance_1K
:
        boost::mpl::identity<distance_1K>
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
