#pragma once
#include <climits>                      // INT_MAX
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/identity.hpp>       // identity
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <dctl/rules/types.hpp>
#include <dctl/preprocessor/has_type.hpp>
#include <dctl/preprocessor/get_type.hpp>

namespace dctl {
namespace rules {

DCTL_PP_DEFINE_HAS_TYPE(max_repetitions)
DCTL_PP_DEFINE_HAS_TYPE(max_3v1_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_2v1_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_2Kv1K_majority_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_3Kv1K_minority_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_LLv1K_majority_moves)

DCTL_PP_DEFINE_GET_TYPE(max_repetitions)
DCTL_PP_DEFINE_GET_TYPE(max_3v1_moves)
DCTL_PP_DEFINE_GET_TYPE(max_2v1_moves)
DCTL_PP_DEFINE_GET_TYPE(max_2Kv1K_majority_moves)
DCTL_PP_DEFINE_GET_TYPE(max_3Kv1K_minority_moves)
DCTL_PP_DEFINE_GET_TYPE(max_LLv1K_majority_moves)

template<typename Rules>
struct draw_traits
{
        typedef Rules rules_type;

        // extract required parameters

        typedef typename boost::mpl::eval_if<
                has_max_repetitions<Rules>,
                get_max_repetitions<Rules>,
                std::integral_constant<int, 3>
        >::type max_repetitions;

        typedef typename boost::mpl::eval_if<
                has_max_3v1_moves<Rules>,
                get_max_3v1_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_3v1_moves;

        typedef typename boost::mpl::eval_if<
                has_max_2v1_moves<Rules>,
                get_max_2v1_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_2v1_moves;

        typedef typename boost::mpl::eval_if<
                has_max_2Kv1K_majority_moves<Rules>,
                get_max_2Kv1K_majority_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_2Kv1K_majority_moves;

        typedef typename boost::mpl::eval_if<
                has_max_3Kv1K_minority_moves<Rules>,
                get_max_3Kv1K_minority_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_3Kv1K_minority_moves;

        typedef typename boost::mpl::eval_if<
                has_max_LLv1K_majority_moves<Rules>,
                get_max_LLv1K_majority_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_LLv1K_majority_moves;

        // compute auxiliary parameters

        typedef typename boost::mpl::eval_if<
                std::is_same<
                        max_repetitions,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_repetitions;

        typedef typename boost::mpl::eval_if<
                std::is_same<
                        max_3v1_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_3v1_moves;

        typedef typename boost::mpl::eval_if<
                std::is_same<
                        max_2v1_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_2v1_moves;

        typedef typename boost::mpl::eval_if<
                std::is_same<
                        max_2Kv1K_majority_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_2Kv1K_majority_moves;

        typedef typename boost::mpl::eval_if<
                std::is_same<
                        max_3Kv1K_minority_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_3Kv1K_minority_moves;

        typedef typename boost::mpl::eval_if<
                std::is_same<
                        max_LLv1K_majority_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_LLv1K_majority_moves;
};

}       // namespace rules
}       // namespace dctl
