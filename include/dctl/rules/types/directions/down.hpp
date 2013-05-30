#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace directions {

struct down
:
        boost::mpl::identity<down>
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
