#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace precedence {

struct quantity
:
        boost::mpl::identity<quantity>
{};

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
