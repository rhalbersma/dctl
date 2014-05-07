#pragma once
#include <dctl/pp/tti/box_type.hpp>             // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/traits/range/king.hpp>     // king
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/tti/has_type.hpp>               // BOOST_TTI_HAS_TYPE

namespace dctl {
namespace rules {
namespace range {

BOOST_TTI_HAS_TYPE(range_move)
DCTL_PP_TTI_BOX_TYPE(range_move)

template<class Rules>
struct move
:
        boost::mpl::eval_if<
                has_type_range_move<Rules>,
                box_type_range_move<Rules>,
                king<Rules>
        >::type
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
