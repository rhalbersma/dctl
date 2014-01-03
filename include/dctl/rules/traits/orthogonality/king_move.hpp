#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>        // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>        // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/types/orthogonality.hpp>           // none

namespace dctl {
namespace rules {
namespace orthogonality {

DCTL_PP_DEFINE_HAS_TYPE(orthogonality_king_move)
DCTL_PP_DEFINE_GET_TYPE(orthogonality_king_move)

template<class Rules>
struct king_move
:
        boost::mpl::eval_if<
                has_type_orthogonality_king_move<Rules>,
                get_type_orthogonality_king_move<Rules>,
                none
        >::type
{};

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
