#pragma once
#include <dctl/rules/traits/max_same_king_moves.hpp>    // max_same_king_push
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <climits>                                      // INT_MAX
#include <type_traits>                                  // integral_constant, is_same

namespace dctl {
namespace rules {

template<class Rules>
struct is_restricted_same_king_moves
:
        boost::mpl::eval_if<
                std::is_same< typename
                        max_same_king_push<Rules>::type,
                        std::integral_constant<int, 0>
                >,
                std::false_type,
                std::true_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
