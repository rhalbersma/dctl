#pragma once
#include <type_traits>                          // false_type
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(is_absolute_king_precedence)
DCTL_PP_DEFINE_GET_TYPE(is_absolute_king_precedence)

template<class Rules>
struct is_absolute_king_precedence
:
        boost::mpl::eval_if<
                has_is_absolute_king_precedence<Rules>,
                get_is_absolute_king_precedence<Rules>,
                std::false_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
