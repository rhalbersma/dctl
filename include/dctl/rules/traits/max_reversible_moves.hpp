#pragma once
#include <climits>                      // INT_MAX
#include <type_traits>                  // integral_constant
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE

namespace dctl {
namespace rules {

BOOST_TTI_HAS_TYPE(max_reversible_moves)
DCTL_PP_TTI_BOX_TYPE(max_reversible_moves)

template<class Rules>
struct max_reversible_moves
:
        boost::mpl::eval_if<
                has_type_max_reversible_moves<Rules>,
                box_type_max_reversible_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type
{};

}       // namespace rules
}       // namespace dctl
