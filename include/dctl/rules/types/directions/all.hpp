#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace directions {

struct all
:
        boost::mpl::identity<all>
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
