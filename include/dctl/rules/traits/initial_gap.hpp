#pragma once
#include <type_traits>                          // integral_constant
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(initial_gap)
DCTL_PP_DEFINE_GET_TYPE(initial_gap)

template<class Rules>
struct initial_gap
:
        boost::mpl::eval_if<
                has_type_initial_gap<Rules>,
                get_type_initial_gap<Rules>,
                std::integral_constant<int, 2>
        >::type
{};

}       // namespace rules
}       // namespace dctl
