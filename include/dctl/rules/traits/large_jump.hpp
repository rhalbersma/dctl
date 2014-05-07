#pragma once
#include <dctl/rules/traits/phase/jump_removal.hpp>     // jump_removal
#include <dctl/rules/traits/directions/is_reversal.hpp> // is_reversal
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/types/phase.hpp>                   // apres_fini
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
                                        jump_removal_phase_t<Rules>,
                                        en_passant
                                >,
                                directions::is_reversal<Rules>
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
