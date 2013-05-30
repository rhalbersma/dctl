#pragma once
#include <type_traits>                          // false_type
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {
namespace directions {

DCTL_PP_DEFINE_HAS_TYPE(directions_is_reversal)
DCTL_PP_DEFINE_GET_TYPE(directions_is_reversal)

template<class Rules>
struct is_reversal
:
        boost::mpl::eval_if<
                has_directions_is_reversal<Rules>,
                get_directions_is_reversal<Rules>,
                std::false_type
        >::type
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
