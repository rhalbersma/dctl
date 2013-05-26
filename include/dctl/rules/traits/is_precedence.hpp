#pragma once
#include <type_traits>                                  // integral_constant, is_same
#include <boost/mpl/logical.hpp>                        // not_
#include <dctl/rules/traits/precedence_jump.hpp>        // precedence_jump
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct is_precedence
:
        std::integral_constant<
                bool,
                boost::mpl::not_<
                        std::is_same< typename
                                precedence_jump<Rules>::type,
                                precedence::none
                        >
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
