#pragma once
#include <climits>                      // INT_MAX
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/identity.hpp>       // identity
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <boost/tti/has_type.hpp>       // BOOST_TTI_HAS_TYPE
#include <dctl/pp/tti/box_type.hpp>     // DCTL_PP_TTI_BOX_TYPE
#include <dctl/rules/types.hpp>

namespace dctl {
namespace rules {

BOOST_TTI_HAS_TYPE(max_repetitions)
BOOST_TTI_HAS_TYPE(max_3v1_moves)
BOOST_TTI_HAS_TYPE(max_2v1_moves)
BOOST_TTI_HAS_TYPE(max_2Kv1K_majority_moves)
BOOST_TTI_HAS_TYPE(max_3Kv1K_minority_moves)
BOOST_TTI_HAS_TYPE(max_LLv1K_majority_moves)

DCTL_PP_TTI_BOX_TYPE(max_repetitions)
DCTL_PP_TTI_BOX_TYPE(max_3v1_moves)
DCTL_PP_TTI_BOX_TYPE(max_2v1_moves)
DCTL_PP_TTI_BOX_TYPE(max_2Kv1K_majority_moves)
DCTL_PP_TTI_BOX_TYPE(max_3Kv1K_minority_moves)
DCTL_PP_TTI_BOX_TYPE(max_LLv1K_majority_moves)

template<class Rules>
struct draw_traits
{
        typedef Rules rules_type;

        // extract required parameters

        typedef typename boost::mpl::eval_if<
                has_type_max_repetitions<Rules>,
                box_type_max_repetitions<Rules>,
                std::integral_constant<int, 3>
        >::type max_repetitions;

        typedef typename boost::mpl::eval_if<
                has_type_max_3v1_moves<Rules>,
                box_type_max_3v1_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_3v1_moves;

        typedef typename boost::mpl::eval_if<
                has_type_max_2v1_moves<Rules>,
                box_type_max_2v1_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_2v1_moves;

        typedef typename boost::mpl::eval_if<
                has_type_max_2Kv1K_majority_moves<Rules>,
                box_type_max_2Kv1K_majority_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_2Kv1K_majority_moves;

        typedef typename boost::mpl::eval_if<
                has_type_max_3Kv1K_minority_moves<Rules>,
                box_type_max_3Kv1K_minority_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_3Kv1K_minority_moves;

        typedef typename boost::mpl::eval_if<
                has_type_max_LLv1K_majority_moves<Rules>,
                box_type_max_LLv1K_majority_moves<Rules>,
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
