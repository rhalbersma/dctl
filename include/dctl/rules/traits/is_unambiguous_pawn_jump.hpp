#pragma once
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/traits/is_en_passant_promotion.hpp>        // promotion
#include <dctl/rules/types/directions.hpp>              // up, down
#include <boost/mpl/logical.hpp>                        // and_, or_
#include <type_traits>                                  // is_same

namespace dctl {
namespace rules {

template<class Rules>
struct is_unambiguous_pawn_jump
:
        std::integral_constant<
                bool,
                boost::mpl::or_<
                        std::is_same< typename
                                directions::pawn_jump<Rules>::type,
                                directions::down
                        >,
                        boost::mpl::and_<
                                std::is_same< typename
                                        directions::pawn_jump<Rules>::type,
                                        directions::up
                                >,
                                std::is_same<
                                        is_en_passant_promotion_t<Rules>,
                                        std::false_type
                                >
                        >
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
