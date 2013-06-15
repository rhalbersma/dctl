#pragma once
#include <type_traits>                                  // is_same
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/sfinae/has_type.hpp>               // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>               // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/directions/pawn_jump.hpp>   // pawn_jump
#include <dctl/rules/types/orthogonality.hpp>           // absolute, none
#include <dctl/rules/types/directions.hpp>              // all

namespace dctl {
namespace rules {
namespace orthogonality {

DCTL_PP_DEFINE_HAS_TYPE(orthogonality_king_jump)
DCTL_PP_DEFINE_GET_TYPE(orthogonality_king_jump)

template<class Rules>
struct king_jump
:
        boost::mpl::eval_if<
                has_type_orthogonality_king_jump<Rules>,
                get_type_orthogonality_king_jump<Rules>,
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
