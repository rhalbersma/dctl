#pragma once
#include <type_traits>                          // false_type
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {
namespace precedence {

DCTL_PP_DEFINE_HAS_TYPE(precedence_is_absolute_king)
DCTL_PP_DEFINE_GET_TYPE(precedence_is_absolute_king)

template<class Rules>
struct is_absolute_king
:
        boost::mpl::eval_if<
                has_type_precedence_is_absolute_king<Rules>,
                get_type_precedence_is_absolute_king<Rules>,
                std::false_type
        >::type
{};

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
