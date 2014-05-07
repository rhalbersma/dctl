#pragma once
#include <dctl/pp/tti/box_type.hpp>                     // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/types/orthogonality.hpp>           // absolute, none
#include <dctl/rules/types/directions.hpp>              // all
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <boost/tti/has_type.hpp>                       // BOOST_TTI_HAS_TYPE
#include <type_traits>                                  // is_same

namespace dctl {
namespace rules {
namespace orthogonality {

BOOST_TTI_HAS_TYPE(orthogonality_king_jump)
DCTL_PP_TTI_BOX_TYPE(orthogonality_king_jump)

template<class Rules>
struct king_jump
:
        boost::mpl::eval_if<
                has_type_orthogonality_king_jump<Rules>,
                box_type_orthogonality_king_jump<Rules>,
                boost::mpl::eval_if<
                        std::is_same< typename
                                directions::pawn_jump<Rules>::type,
                                directions::all
                        >,
                        absolute,
                        none
                >
        >::type
{};

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
