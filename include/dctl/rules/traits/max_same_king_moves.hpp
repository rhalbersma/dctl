#pragma once
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <climits>                      // INT_MAX
#include <type_traits>                  // integral_constant

namespace dctl {
namespace rules {

BOOST_TTI_HAS_TYPE(max_same_king_push)
DCTL_PP_TTI_BOX_TYPE(max_same_king_push)

template<class Rules>
struct max_same_king_push
:
        boost::mpl::eval_if<
                has_type_max_same_king_push<Rules>,
                box_type_max_same_king_push<Rules>,
                std::integral_constant<int, 0>
        >::type
{};

}       // namespace rules
}       // namespace dctl
