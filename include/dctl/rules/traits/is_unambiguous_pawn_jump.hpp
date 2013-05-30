#pragma once
#include <type_traits>                                  // is_same
#include <boost/mpl/logical.hpp>                        // and_, or_
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/traits/phase/promotion.hpp>        // promotion
#include <dctl/rules/types/directions.hpp>              // up, down
#include <dctl/rules/types/phase.hpp>                   // apres_fini

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
                                std::is_same< typename
                                        phase::promotion<Rules>::type,
                                        phase::apres_fini
                                >
                        >
                >::value
        >
{};

}       // namespace rules
}       // namespace dctl
