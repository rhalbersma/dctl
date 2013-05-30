#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace orthogonality {

struct relative
:
        boost::mpl::identity<relative>
{};

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
