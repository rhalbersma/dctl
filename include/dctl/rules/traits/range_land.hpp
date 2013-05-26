#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/range_king.hpp>     // range_king

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(range_land)
DCTL_PP_DEFINE_GET_TYPE(range_land)

template<class Rules>
struct range_land
:
        boost::mpl::eval_if<
                has_range_land<Rules>,
                get_range_land<Rules>,
                range_king<Rules>
        >::type
{};

}       // namespace rules
}       // namespace dctl
