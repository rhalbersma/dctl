#pragma once
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/types/phase.hpp>   // apres_fini

namespace dctl {
namespace rules {
namespace phase {

BOOST_TTI_HAS_TYPE(phase_promotion)
DCTL_PP_TTI_BOX_TYPE(phase_promotion)

template<class Rules>
struct promotion
:
        boost::mpl::eval_if<
                has_type_phase_promotion<Rules>,
                box_type_phase_promotion<Rules>,
                apres_fini
        >::type
{};

}       // namespace phase
}       // namespace rules
}       // namespace dctl
