#pragma once
#include <dctl/pp/tti/box_type.hpp>             // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/traits/range/land.hpp>     // land
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/tti/has_type.hpp>               // BOOST_TTI_HAS_TYPE

namespace dctl {
namespace rules {
namespace range {

BOOST_TTI_HAS_TYPE(range_halt)
DCTL_PP_TTI_BOX_TYPE(range_halt)

template<class Rules>
struct halt
:
        boost::mpl::eval_if<
                has_type_range_halt<Rules>,
                box_type_range_halt<Rules>,
                land<Rules>
        >::type
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
