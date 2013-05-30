#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace directions {

struct orth
:
        boost::mpl::identity<orth>
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
