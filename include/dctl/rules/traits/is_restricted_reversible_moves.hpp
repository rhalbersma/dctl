#pragma once
#include <climits>                                      // INT_MAX
#include <type_traits>                                  // integral_constant, is_same
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/rules/traits/max_reversible_moves.hpp>   // max_reversible_moves

namespace dctl {
namespace rules {

template<typename Rules>
struct is_restricted_reversible_moves
:
        boost::mpl::eval_if<
                std::is_same<typename
                        max_reversible_moves<Rules>::type,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
