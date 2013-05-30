#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace orthogonality {

struct absolute
:
        boost::mpl::identity<absolute>
{};

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
