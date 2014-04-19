#pragma once
#include <type_traits>                  // false_type
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE

namespace dctl {
namespace rules {
namespace precedence {

BOOST_TTI_HAS_TYPE(precedence_is_relative_king)
DCTL_PP_TTI_BOX_TYPE(precedence_is_relative_king)

template<class Rules>
struct is_relative_king
:
        boost::mpl::eval_if<
                has_type_precedence_is_relative_king<Rules>,
                box_type_precedence_is_relative_king<Rules>,
                std::false_type
        >::type
{};

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
