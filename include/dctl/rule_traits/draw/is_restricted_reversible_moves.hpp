#pragma once
#include <dctl/rule_traits/draw/max_reversible_moves.hpp>       // max_reversible_moves
#include <boost/mpl/eval_if.hpp>                                // eval_if
#include <climits>                                              // INT_MAX
#include <type_traits>                                          // integral_constant, is_same

namespace dctl {
namespace rules {

template<class Rules>
struct is_restricted_reversible_moves
:
        boost::mpl::eval_if<
                std::is_same< typename
                        max_reversible_moves<Rules>::type,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
