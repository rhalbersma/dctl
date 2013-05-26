#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/range_land.hpp>     // range_land

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(range_halt)
DCTL_PP_DEFINE_GET_TYPE(range_halt)

template<class Rules>
struct range_halt
:
        boost::mpl::eval_if<
                has_range_halt<Rules>,
                get_range_halt<Rules>,
                range_land<Rules>
        >::type
{};

}       // namespace rules
}       // namespace dctl
