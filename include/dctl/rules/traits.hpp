#pragma once
#include <climits>                      // INT_MAX
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/identity.hpp>       // identity
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <dctl/rules/enum.hpp>
#include <dctl/preprocessor/has_type.hpp>
#include <dctl/preprocessor/get_type.hpp>

#include <dctl/rules/traits/king_range.hpp>
#include <dctl/rules/traits/pawn_jump_directions.hpp>
#include <dctl/rules/traits/jump_precedence.hpp>

#include <dctl/rules/traits/land_range.hpp>
#include <dctl/rules/traits/halt_range.hpp>
#include <dctl/rules/traits/king_jump_orthogonality.hpp>
#include <dctl/rules/traits/king_move_orthogonality.hpp>
#include <dctl/rules/traits/is_pawns_jump_kings.hpp>
#include <dctl/rules/traits/is_jump_direction_reversal.hpp>
#include <dctl/rules/traits/jump_removal.hpp>
#include <dctl/rules/traits/pawn_promotion.hpp>
#include <dctl/rules/traits/is_absolute_king_precedence.hpp>
#include <dctl/rules/traits/is_relative_king_precedence.hpp>

namespace dctl {
namespace rules {

// intermediate capture directions
template<typename T>
struct turn_directions
:
        boost::mpl::identity<T>
{};

template<>
struct turn_directions<directions::orth>
:
        boost::mpl::identity<directions::diag>
{};

DCTL_PP_DEFINE_HAS_TYPE(initial_gap)
DCTL_PP_DEFINE_HAS_TYPE(max_repetitions)
DCTL_PP_DEFINE_HAS_TYPE(max_reversible_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_3v1_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_2v1_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_same_king_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_2Kv1K_majority_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_3Kv1K_minority_moves)
DCTL_PP_DEFINE_HAS_TYPE(max_LLv1K_majority_moves)

DCTL_PP_DEFINE_GET_TYPE(initial_gap)
DCTL_PP_DEFINE_GET_TYPE(max_repetitions)
DCTL_PP_DEFINE_GET_TYPE(max_reversible_moves)
DCTL_PP_DEFINE_GET_TYPE(max_3v1_moves)
DCTL_PP_DEFINE_GET_TYPE(max_2v1_moves)
DCTL_PP_DEFINE_GET_TYPE(max_same_king_moves)
DCTL_PP_DEFINE_GET_TYPE(max_2Kv1K_majority_moves)
DCTL_PP_DEFINE_GET_TYPE(max_3Kv1K_minority_moves)
DCTL_PP_DEFINE_GET_TYPE(max_LLv1K_majority_moves)

template<typename Rules>
struct traits
{
        typedef Rules rules_type;

        // extract required parameters

        typedef typename boost::mpl::eval_if<
                has_initial_gap<Rules>,
                get_initial_gap<Rules>,
                std::integral_constant<int, 2>
        >::type initial_gap;

        typedef typename boost::mpl::eval_if<
                has_max_repetitions<Rules>,
                get_max_repetitions<Rules>,
                std::integral_constant<int, 3>
        >::type max_repetitions;

        typedef typename boost::mpl::eval_if<
                has_max_reversible_moves<Rules>,
                get_max_reversible_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_reversible_moves;

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
                has_max_same_king_moves<Rules>,
                get_max_same_king_moves<Rules>,
                std::integral_constant<int, INT_MAX>
        >::type max_same_king_moves;

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
                std::is_same<typename king_jump_orthogonality<Rules>::type, orthogonality::none>,
                boost::mpl::identity<directions::diag>,
                boost::mpl::identity<directions::all >
        >::type king_jump_directions;

        typedef typename turn_directions<
                king_jump_directions
        >::type king_turn_directions;

        typedef typename turn_directions<
                typename pawn_jump_directions<Rules>::type
        >::type pawn_turn_directions;

        typedef std::integral_constant<
                bool,
                boost::mpl::or_<
                        std::is_same<typename pawn_jump_directions<Rules>::type, directions::down>,
                        boost::mpl::and_<
                                std::is_same<typename pawn_jump_directions<Rules>::type, directions::up>,
                                std::is_same<typename pawn_promotion<Rules>::type, promotion::apres_fini>
                        >
                >::value
        > is_unambiguous_pawn_jump;

        typedef typename boost::mpl::eval_if<
                boost::mpl::or_<
                        boost::mpl::and_<
                                std::is_same<typename jump_removal<Rules>::type, removal::apres_fini>,
                                std::is_same<typename is_jump_direction_reversal<Rules>::type, std::true_type>
                        >,
                        std::is_same<typename pawn_jump_directions<Rules>::type, directions::all>
                >,
                std::integral_constant<int, 3>,
                std::integral_constant<int, 4>
        >::type large_jump;

        typedef std::integral_constant<
                bool,
                boost::mpl::not_<
                        std::is_same<typename jump_precedence<Rules>::type, precedence::none>
                >::value
        > is_precedence;

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
                        max_reversible_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_reversible_moves;

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
                        max_same_king_moves,
                        std::integral_constant<int, INT_MAX>
                >,
                std::false_type,
                std::true_type
        >::type is_restricted_same_king_moves;

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

//+----------------------------------------------------------------------------+
//|      Capture ambiguity                                                     |
//+----------------------------------------------------------------------------+

template<typename>
struct is_remove_duplicates
:
        std::true_type
{};

}       // namespace rules
}       // namespace dctl
