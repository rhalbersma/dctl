#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace range {

struct distance_1
:
        boost::mpl::identity<distance_1>
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
