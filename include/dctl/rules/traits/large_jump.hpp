#pragma once
#include <dctl/rules/traits/is_en_passant_jump_removal.hpp>     // jump_removal
#include <dctl/rules/traits/is_reversible_king_jump_direction.hpp> // is_reversible_king_jump_direction
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/types/directions.hpp>              // all
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <boost/mpl/logical.hpp>                        // and_, or_
#include <type_traits>                                  // integral_constant, is_same, true_type

namespace dctl {
namespace rules {

template<class Rules>
struct large_jump
:
        boost::mpl::eval_if<
                boost::mpl::or_<
                        boost::mpl::and_<
                                std::is_same<
                                        is_en_passant_jump_removal_t<Rules>,
                                        std::true_type
                                >,
                                is_reversible_king_jump_direction_t<Rules>
                        >,
                        std::is_same< typename
                                directions::pawn_jump<Rules>::type,
                                directions::all
                        >
                >,
                std::integral_constant<int, 3>,
                std::integral_constant<int, 4>
        >::type
{};

}       // namespace rules
}       // namespace dctl
