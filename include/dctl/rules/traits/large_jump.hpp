#pragma once
#include <type_traits>
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/mpl/logical.hpp>
#include <dctl/rules/traits/captures_removal.hpp>
#include <dctl/rules/traits/is_jump_direction_reversal.hpp>
#include <dctl/rules/traits/pawn_jump_directions.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct large_jump
:
        boost::mpl::eval_if<
                boost::mpl::or_<
                        boost::mpl::and_<
                                std::is_same< typename
                                        captures_removal<Rules>::type,
                                        removal::apres_fini
                                >,
                                std::is_same< typename
                                        is_jump_direction_reversal<Rules>::type,
                                        std::true_type
                                >
                        >,
                        std::is_same< typename
                                pawn_jump_directions<Rules>::type,
                                directions::all
                        >
                >,
                std::integral_constant<int, 3>,
                std::integral_constant<int, 3>
        >::type
{};

}       // namespace rules
}       // namespace dctl
