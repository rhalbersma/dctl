#pragma once
#include <type_traits>                  // true_type
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <dctl/pp/tti/has_template.hpp> // DCTL_PP_TTI_HAS_TEMPLATE
#include <dctl/pp/tti/box_template.hpp> // DCTL_PP_TTI_BOX_TEMPLATE

namespace dctl {
namespace rules {

DCTL_PP_TTI_HAS_TEMPLATE(can_jump)
DCTL_PP_TTI_BOX_TEMPLATE(can_jump)

template<class Rules, class Attacker, class Victim>
struct can_jump
:
        boost::mpl::eval_if<
                has_template_can_jump<Rules, Attacker, Victim>,
                box_template_can_jump<Rules, Attacker, Victim>,
                std::true_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
