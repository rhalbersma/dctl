#pragma once
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {
namespace rules {
namespace phase {

struct apres_fini
:
        boost::mpl::identity<apres_fini>
{};

}       // namespace phase
}       // namespace rules
}       // namespace dctl
