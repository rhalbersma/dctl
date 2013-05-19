#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/king_range.hpp>     // king_range

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(land_range)
DCTL_PP_DEFINE_GET_TYPE(land_range)

template<typename Rules>
struct land_range
:
        boost::mpl::eval_if<
                has_land_range<Rules>,
                get_land_range<Rules>,
                king_range<Rules>
        >::type
{};

}       // namespace rules
}       // namespace dctl
