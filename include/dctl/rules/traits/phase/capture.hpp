#pragma once
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/types/phase.hpp>   // apres_fini

namespace dctl {
namespace rules {
namespace phase {

BOOST_TTI_HAS_TYPE(phase_capture)
DCTL_PP_TTI_BOX_TYPE(phase_capture)

template<class Rules>
struct capture
:
        boost::mpl::eval_if<
                has_type_phase_capture<Rules>,
                box_type_phase_capture<Rules>,
                apres_fini
        >::type
{};

}       // namespace phase
}       // namespace rules
}       // namespace dctl
