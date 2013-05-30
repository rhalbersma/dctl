#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace phase {

struct en_passant
:
        boost::mpl::identity<en_passant>
{};

}       // namespace phase
}       // namespace rules
}       // namespace dctl
