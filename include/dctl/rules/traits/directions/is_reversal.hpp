#pragma once
#include <type_traits>                  // false_type
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE

namespace dctl {
namespace rules {
namespace directions {

BOOST_TTI_HAS_TYPE(directions_is_reversal)
DCTL_PP_TTI_BOX_TYPE(directions_is_reversal)

template<class Rules>
struct is_reversal
:
        boost::mpl::eval_if<
                has_type_directions_is_reversal<Rules>,
                box_type_directions_is_reversal<Rules>,
                std::false_type
        >::type
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
