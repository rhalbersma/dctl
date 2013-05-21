#pragma once
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/enum.hpp>                  // apres_fini

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(captures_removal)
DCTL_PP_DEFINE_GET_TYPE(captures_removal)

template<class Rules>
struct captures_removal
:
        boost::mpl::eval_if<
                has_captures_removal<Rules>,
                get_captures_removal<Rules>,
                removal::apres_fini
        >::type
{};

}       // namespace rules
}       // namespace dctl
