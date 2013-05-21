#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/land_range.hpp>     // land_range

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(halt_range)
DCTL_PP_DEFINE_GET_TYPE(halt_range)

template<class Rules>
struct halt_range
:
        boost::mpl::eval_if<
                has_halt_range<Rules>,
                get_halt_range<Rules>,
                land_range<Rules>
        >::type
{};

}       // namespace rules
}       // namespace dctl
