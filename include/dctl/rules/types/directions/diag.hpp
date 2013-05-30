#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace directions {

struct diag
:
        boost::mpl::identity<diag>
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
