#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/has_type.hpp>               // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>               // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(orthogonality_king_move)
DCTL_PP_DEFINE_GET_TYPE(orthogonality_king_move)

template<class Rules>
struct orthogonality_king_move
:
        boost::mpl::eval_if<
                has_orthogonality_king_move<Rules>,
                get_orthogonality_king_move<Rules>,
                orthogonality::none
        >::type
{};

}       // namespace rules
}       // namespace dctl
