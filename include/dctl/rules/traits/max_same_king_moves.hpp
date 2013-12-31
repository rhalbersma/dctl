#pragma once
#include <climits>                              // INT_MAX
#include <type_traits>                          // integral_constant
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(max_same_king_moves)
DCTL_PP_DEFINE_GET_TYPE(max_same_king_moves)

template<class Rules>
struct max_same_king_moves
:
        boost::mpl::eval_if<
                has_type_max_same_king_moves<Rules>,
                get_type_max_same_king_moves<Rules>,
                std::integral_constant<int, 0>
        >::type
{};

}       // namespace rules
}       // namespace dctl
