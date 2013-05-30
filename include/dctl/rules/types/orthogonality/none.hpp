#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace orthogonality {

struct none
:
        boost::mpl::identity<none>
{};

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
