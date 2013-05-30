#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/types/phase.hpp>           // apres_fini

namespace dctl {
namespace rules {
namespace phase {

DCTL_PP_DEFINE_HAS_TYPE(phase_capture)
DCTL_PP_DEFINE_GET_TYPE(phase_capture)

template<class Rules>
struct capture
:
        boost::mpl::eval_if<
                has_phase_capture<Rules>,
                get_phase_capture<Rules>,
                apres_fini
        >::type
{};

}       // namespace phase
}       // namespace rules
}       // namespace dctl
