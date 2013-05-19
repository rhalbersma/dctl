#pragma once
#include <type_traits>
#include <boost/mpl/eval_if.hpp>                        // eval_if
#include <dctl/preprocessor/has_type.hpp>               // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>               // DCTL_PP_DEFINE_GET_TYPE
#include <dctl/rules/traits/pawn_jump_directions.hpp>   // pawn_jump_directions
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(king_jump_orthogonality)
DCTL_PP_DEFINE_GET_TYPE(king_jump_orthogonality)

template<typename Rules>
struct king_jump_orthogonality
:
        boost::mpl::eval_if<
                has_king_jump_orthogonality<Rules>,
                get_king_jump_orthogonality<Rules>,
                boost::mpl::eval_if<
                        std::is_same<typename
                                pawn_jump_directions<Rules>::type,
                                directions::all
                        >,
                        orthogonality::absolute,
                        orthogonality::none
                >
        >::type
{};

}       // namespace rules
}       // namespace dctl
