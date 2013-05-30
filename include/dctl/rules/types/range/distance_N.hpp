#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace range {

struct distance_N
:
        boost::mpl::identity<distance_N>
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
