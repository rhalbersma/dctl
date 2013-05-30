#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace precedence {

struct quality
:
        boost::mpl::identity<quality>
{};

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
