#pragma once
#include <type_traits>
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/mpl/logical.hpp>
#include <dctl/rules/traits/phase_capture.hpp>
#include <dctl/rules/traits/is_jump_direction_reversal.hpp>
#include <dctl/rules/traits/directions_pawn_jump.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct large_jump
:
        boost::mpl::eval_if<
                boost::mpl::or_<
                        boost::mpl::and_<
                                std::is_same< typename
                                        phase_capture<Rules>::type,
                                        phase::apres_fini
                                >,
                                std::is_same< typename
                                        is_jump_direction_reversal<Rules>::type,
                                        std::true_type
                                >
                        >,
                        std::is_same< typename
                                directions_pawn_jump<Rules>::type,
                                directions::all
                        >
                >,
                std::integral_constant<int, 3>,
                std::integral_constant<int, 3>
        >::type
{};

}       // namespace rules
}       // namespace dctl
