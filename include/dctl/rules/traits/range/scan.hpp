#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>        // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>        // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/range/king.hpp>             // king

namespace dctl {
namespace rules {
namespace range {

DCTL_PP_DEFINE_HAS_TYPE(range_scan)
DCTL_PP_DEFINE_GET_TYPE(range_scan)

template<class Rules>
struct scan
:
        boost::mpl::eval_if<
                has_type_range_scan<Rules>,
                get_type_range_scan<Rules>,
                king<Rules>
        >::type
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
