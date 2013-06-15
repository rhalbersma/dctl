#pragma once
#include <type_traits>                                  // true_type
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/sfinae/has_template.hpp>    // DCTL_PP_DEFINE_HAS_TEMPLATE
#include <dctl/preprocessor/sfinae/get_template.hpp>    // DCTL_PP_DEFINE_GET_TEMPLATE

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TEMPLATE(can_jump)
DCTL_PP_DEFINE_GET_TEMPLATE(can_jump)

template<class Rules, class Attacker, class Victim>
struct can_jump
:
        boost::mpl::eval_if<
                has_template_can_jump<Rules, Attacker, Victim>,
                get_template_can_jump<Rules, Attacker, Victim>,
                std::true_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
