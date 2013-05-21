#pragma once
#include <type_traits>                                  // integral_constant, is_same
#include <boost/mpl/logical.hpp>                        // not_
#include <dctl/rules/traits/jump_precedence.hpp>        // jump_precedence
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
                                jump_precedence<Rules>::type,
                                precedence::none
                        >
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
