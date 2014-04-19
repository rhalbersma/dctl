#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/tti/has_type.hpp>               // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>             // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/types/orthogonality.hpp>   // none

namespace dctl {
namespace rules {
namespace orthogonality {

BOOST_TTI_HAS_TYPE(orthogonality_king_move)
DCTL_PP_TTI_BOX_TYPE(orthogonality_king_move)

template<class Rules>
struct king_move
:
        boost::mpl::eval_if<
                has_type_orthogonality_king_move<Rules>,
                box_type_orthogonality_king_move<Rules>,
                none
        >::type
{};

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
