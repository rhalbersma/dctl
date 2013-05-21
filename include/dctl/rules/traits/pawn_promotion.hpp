#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/enum.hpp>                  // apres_fini

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(pawn_promotion)
DCTL_PP_DEFINE_GET_TYPE(pawn_promotion)

template<class Rules>
struct pawn_promotion
:
        boost::mpl::eval_if<
                has_pawn_promotion<Rules>,
                get_pawn_promotion<Rules>,
                promotion::apres_fini
        >::type
{};

}       // namespace rules
}       // namespace dctl
