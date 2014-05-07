#pragma once
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <type_traits>                  // integral_constant

namespace dctl {
namespace rules {

BOOST_TTI_HAS_TYPE(initial_gap)
DCTL_PP_TTI_BOX_TYPE(initial_gap)

template<class Rules>
struct initial_gap
:
        boost::mpl::eval_if<
                has_type_initial_gap<Rules>,
                box_type_initial_gap<Rules>,
                std::integral_constant<int, 2>
        >::type
{};

}       // namespace rules
}       // namespace dctl
