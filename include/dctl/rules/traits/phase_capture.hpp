#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/enum.hpp>                  // apres_fini

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(phase_capture)
DCTL_PP_DEFINE_GET_TYPE(phase_capture)

template<class Rules>
struct phase_capture
:
        boost::mpl::eval_if<
                has_phase_capture<Rules>,
                get_phase_capture<Rules>,
                phase::apres_fini
        >::type
{};

}       // namespace rules
}       // namespace dctl
