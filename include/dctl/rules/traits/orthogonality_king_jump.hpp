#pragma once
#include <type_traits>
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/has_type.hpp>               // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>               // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/directions_pawn_jump.hpp>   // directions_pawn_jump
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(orthogonality_king_jump)
DCTL_PP_DEFINE_GET_TYPE(orthogonality_king_jump)

template<class Rules>
struct orthogonality_king_jump
:
        boost::mpl::eval_if<
                has_orthogonality_king_jump<Rules>,
                get_orthogonality_king_jump<Rules>,
                boost::mpl::eval_if<
                        std::is_same< typename
                                directions_pawn_jump<Rules>::type,
                                directions::all
                        >,
                        orthogonality::absolute,
                        orthogonality::none
                >
        >::type
{};

}       // namespace rules
}       // namespace dctl
