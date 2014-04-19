#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/tti/has_type.hpp>               // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>             // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/traits/range/scan.hpp>     // scan

namespace dctl {
namespace rules {
namespace range {

BOOST_TTI_HAS_TYPE(range_land)
DCTL_PP_TTI_BOX_TYPE(range_land)

template<class Rules>
struct land
:
        boost::mpl::eval_if<
                has_type_range_land<Rules>,
                box_type_range_land<Rules>,
                scan<Rules>
        >::type
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
