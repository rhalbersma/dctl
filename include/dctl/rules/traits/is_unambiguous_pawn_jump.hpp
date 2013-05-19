#pragma once
#include <type_traits>
#include <boost/mpl/logical.hpp>                // eval_if
#include <dctl/rules/traits/pawn_jump_directions.hpp>
#include <dctl/rules/traits/pawn_promotion.hpp>

namespace dctl {
namespace rules {

template<typename Rules>
struct is_unambiguous_pawn_jump
:
        std::integral_constant<
                bool,
                boost::mpl::or_<
                        std::is_same<typename pawn_jump_directions<Rules>::type, directions::down>,
                        boost::mpl::and_<
                                std::is_same<typename pawn_jump_directions<Rules>::type, directions::up>,
                                std::is_same<typename pawn_promotion<Rules>::type, promotion::apres_fini>
                        >
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
