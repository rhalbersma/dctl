#pragma once
#include <type_traits>                          // true_type
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(is_pawns_jump_kings)
DCTL_PP_DEFINE_GET_TYPE(is_pawns_jump_kings)

template<typename Rules>
struct is_pawns_jump_kings
:
        boost::mpl::eval_if<
                has_is_pawns_jump_kings<Rules>,
                get_is_pawns_jump_kings<Rules>,
                std::true_type
        >::type
{};

}       // namespace rules
}       // namespace dctl
