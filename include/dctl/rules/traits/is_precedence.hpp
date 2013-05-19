#pragma once
#include <type_traits>
#include <boost/mpl/logical.hpp>                        // eval_if
#include <dctl/rules/traits/jump_precedence.hpp>

namespace dctl {
namespace rules {

template<typename Rules>
struct is_precedence
:
        std::integral_constant<
                bool,
                boost::mpl::not_<
                        std::is_same<typename jump_precedence<Rules>::type, precedence::none>
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
