#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>        // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>        // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/range/land.hpp>             // land

namespace dctl {
namespace rules {
namespace range {

DCTL_PP_DEFINE_HAS_TYPE(range_halt)
DCTL_PP_DEFINE_GET_TYPE(range_halt)

template<class Rules>
struct halt
:
        boost::mpl::eval_if<
                has_type_range_halt<Rules>,
                get_type_range_halt<Rules>,
                land<Rules>
        >::type
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
