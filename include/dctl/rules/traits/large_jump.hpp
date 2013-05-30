#pragma once
#include <type_traits>                                  // integral_constant, is_same, true_type
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <boost/mpl/logical.hpp>                        // and_, or_
#include <dctl/rules/traits/phase/capture.hpp>          // capture
#include <dctl/rules/traits/directions/is_reversal.hpp> // is_reversal
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/types/phase.hpp>                   // apres_fini
#include <dctl/rules/types/directions.hpp>              // all

namespace dctl {
namespace rules {

template<class Rules>
struct large_jump
:
        boost::mpl::eval_if<
                boost::mpl::or_<
                        boost::mpl::and_<
                                std::is_same< typename
                                        phase::capture<Rules>::type,
                                        phase::apres_fini
                                >,
                                std::is_same< typename
                                        directions::is_reversal<Rules>::type,
                                        std::true_type
                                >
                        >,
                        std::is_same< typename
                                directions::pawn_jump<Rules>::type,
                                directions::all
                        >
                >,
                std::integral_constant<int, 3>,
                std::integral_constant<int, 3>
        >::type
{};

}       // namespace rules
}       // namespace dctl
