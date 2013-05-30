#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace precedence {

struct none
:
        boost::mpl::identity<none>
{};

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
