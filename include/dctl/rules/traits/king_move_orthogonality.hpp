#pragma once
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/has_type.hpp>               // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>               // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(king_move_orthogonality)
DCTL_PP_DEFINE_GET_TYPE(king_move_orthogonality)

template<class Rules>
struct king_move_orthogonality
:
        boost::mpl::eval_if<
                has_king_move_orthogonality<Rules>,
                get_king_move_orthogonality<Rules>,
                orthogonality::none
        >::type
{};

}       // namespace rules
}       // namespace dctl
