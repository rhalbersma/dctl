#pragma once
#include <climits>                              // INT_MAX
#include <type_traits>                          // integral_constant
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(max_reversible_moves)
DCTL_PP_DEFINE_GET_TYPE(max_reversible_moves)

template<class Rules>
struct max_reversible_moves
:
        boost::mpl::eval_if<
                has_type_max_reversible_moves<Rules>,
                get_type_max_reversible_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type
{};

}       // namespace rules
}       // namespace dctl
