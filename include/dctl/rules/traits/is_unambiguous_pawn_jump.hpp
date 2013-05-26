#pragma once
#include <type_traits>
#include <boost/mpl/logical.hpp>                // eval_if
#include <dctl/rules/traits/directions_pawn_jump.hpp>
#include <dctl/rules/traits/phase_promotion.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct is_unambiguous_pawn_jump
:
        std::integral_constant<
                bool,
                boost::mpl::or_<
                        std::is_same< typename
                                directions_pawn_jump<Rules>::type,
                                directions::down
                        >,
                        boost::mpl::and_<
                                std::is_same< typename
                                        directions_pawn_jump<Rules>::type,
                                        directions::up
                                >,
                                std::is_same< typename
                                        phase_promotion<Rules>::type,
                                        phase::apres_fini
                                >
                        >
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
