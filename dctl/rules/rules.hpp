#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/identity.hpp>       // identity
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // and_, not_, or_

#include <dctl/rules/enum.hpp>
#include <dctl/capture/value_fwd.hpp>
#include <dctl/guarded/parameter.hpp>     // Boost.Parameter

namespace dctl {
namespace rules {

//+----------------------------------------------------------------------------+
//|      Drawing rules                                                         |
//+----------------------------------------------------------------------------+

// restricted repeated positions
template<typename> struct is_restricted_repetitions             { enum { value = true }; };

// maximum number of repeated positions
template<typename> struct max_repetitions                       { enum { value =  3 }; };       // (FMJD rule 6.1)

// restricted consecutive non-conversion moves
template<typename> struct is_restricted_reversible_moves        { enum { value = false }; };

// maximum allowed consecutive non-conversion moves
template<typename> struct max_reversible_moves                  { enum { value = 50 }; };       // (FMJD rule 6.2)

// other drawing rules
template<typename> struct max_3v1_moves                         { enum { value = 32 }; };       // (FMJD rule 6.3)
template<typename> struct max_2v1_moves                         { enum { value = 10 }; };       // (FMJD rule 6.4)

//+----------------------------------------------------------------------------+
//|      Capture ambiguity                                                     |
//+----------------------------------------------------------------------------+

template<typename>
struct is_check_jump_uniqueness
:
        boost::mpl::true_
{};

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

BOOST_PARAMETER_TEMPLATE_KEYWORD(king_range)
BOOST_PARAMETER_TEMPLATE_KEYWORD(pawn_jump_directions)
BOOST_PARAMETER_TEMPLATE_KEYWORD(jump_precedence)

BOOST_PARAMETER_TEMPLATE_KEYWORD(is_restricted_same_king_moves)
BOOST_PARAMETER_TEMPLATE_KEYWORD(max_same_king_moves)
BOOST_PARAMETER_TEMPLATE_KEYWORD(land_range)
BOOST_PARAMETER_TEMPLATE_KEYWORD(halt_range)
BOOST_PARAMETER_TEMPLATE_KEYWORD(king_jump_orthogonality)
BOOST_PARAMETER_TEMPLATE_KEYWORD(king_move_orthogonality)
BOOST_PARAMETER_TEMPLATE_KEYWORD(is_pawns_jump_kings)
BOOST_PARAMETER_TEMPLATE_KEYWORD(is_jump_direction_reversal)
BOOST_PARAMETER_TEMPLATE_KEYWORD(jump_removal)
BOOST_PARAMETER_TEMPLATE_KEYWORD(pawn_promotion)
BOOST_PARAMETER_TEMPLATE_KEYWORD(is_absolute_king_precedence)
BOOST_PARAMETER_TEMPLATE_KEYWORD(is_relative_king_precedence)
BOOST_PARAMETER_TEMPLATE_KEYWORD(initial_dmz)

typedef boost::parameter::parameters<
        // required parameters

        boost::parameter::required<tag::king_range>,
        boost::parameter::required<tag::pawn_jump_directions>,
        boost::parameter::required<tag::jump_precedence>,

        // optional parameters

        boost::parameter::optional<tag::is_restricted_same_king_moves>,
        boost::parameter::optional<tag::max_same_king_moves>,
        boost::parameter::optional<tag::land_range>,
        boost::parameter::optional<tag::halt_range>,
        boost::parameter::optional<tag::king_jump_orthogonality>,
        boost::parameter::optional<tag::king_move_orthogonality>,
        boost::parameter::optional<tag::is_pawns_jump_kings>,
        boost::parameter::optional<tag::is_jump_direction_reversal>,
        boost::parameter::optional<tag::jump_removal>,
        boost::parameter::optional<tag::pawn_promotion>,
        boost::parameter::optional<tag::is_absolute_king_precedence>,
        boost::parameter::optional<tag::is_relative_king_precedence>,
        boost::parameter::optional<tag::initial_dmz>
> Signature;

template
<
        class rules_tag,
        class A0 = boost::parameter::void_,
        class A1 = boost::parameter::void_,
        class A2 = boost::parameter::void_,
        class A3 = boost::parameter::void_,
        class A4 = boost::parameter::void_,
        class A5 = boost::parameter::void_,
        class A6 = boost::parameter::void_,
        class A7 = boost::parameter::void_,
        class A8 = boost::parameter::void_,
        class A9 = boost::parameter::void_,
        class A10 = boost::parameter::void_,
        class A11 = boost::parameter::void_,
        class A12 = boost::parameter::void_,
        class A13 = boost::parameter::void_,
        class A14 = boost::parameter::void_,
        class A15 = boost::parameter::void_
>
struct Rules
{
        // create argument pack
        typedef typename Signature::bind<
                A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,  
                A8,  A9, A10, A11, A12, A13, A14, A15
        >::type args;

        // extract required parameters

        typedef typename boost::parameter::value_type<
                args, tag::king_range
        >::type king_range;

        typedef typename boost::parameter::value_type<
                args, tag::pawn_jump_directions
        >::type pawn_jump_directions;

        typedef typename boost::parameter::value_type<
                args, tag::jump_precedence
        >::type jump_precedence;

        // extract optional parameters

        typedef typename boost::parameter::value_type<
                args, tag::is_restricted_same_king_moves, boost::mpl::false_
        >::type is_restricted_same_king_moves;

        typedef typename boost::parameter::value_type<
                args, tag::max_same_king_moves, boost::mpl::int_<0>
        >::type max_same_king_moves;

        typedef typename boost::parameter::value_type<
                args, tag::land_range, king_range
        >::type land_range;

        typedef typename boost::parameter::value_type<
                args, tag::halt_range, land_range
        >::type halt_range;

        typedef typename boost::parameter::value_type<
                args, tag::king_jump_orthogonality, typename 
                boost::mpl::eval_if<
                        std::is_same<pawn_jump_directions, directions::all>,
                        boost::mpl::identity<orthogonality::absolute>,
                        boost::mpl::identity<orthogonality::none    >
                >::type
        >::type king_jump_orthogonality;

        typedef typename boost::parameter::value_type<
                args, tag::king_move_orthogonality, orthogonality::none
        >::type king_move_orthogonality;

        typedef typename boost::parameter::value_type<
                args, tag::is_pawns_jump_kings, boost::mpl::true_
        >::type is_pawns_jump_kings;

        typedef typename boost::parameter::value_type<
                args, tag::is_jump_direction_reversal, boost::mpl::false_
        >::type is_jump_direction_reversal;

        typedef typename boost::parameter::value_type<
                args, tag::jump_removal, removal::apres_fini
        >::type jump_removal;

        typedef typename boost::parameter::value_type<
                args, tag::pawn_promotion, promotion::apres_fini
        >::type pawn_promotion;

        typedef typename boost::parameter::value_type<
                args, tag::is_absolute_king_precedence, boost::mpl::false_
        >::type is_absolute_king_precedence;

        typedef typename boost::parameter::value_type<
                args, tag::is_relative_king_precedence, boost::mpl::false_
        >::type is_relative_king_precedence;

        typedef typename boost::parameter::value_type<
                args, tag::initial_dmz, boost::mpl::int_<2>
        >::type initial_dmz;

        // compute auxiliary parameters

        typedef typename boost::mpl::eval_if<
                std::is_same<king_jump_orthogonality, orthogonality::none>,
                boost::mpl::identity<directions::diag>,
                boost::mpl::identity<directions::all >
        >::type king_jump_directions;

        typedef typename turn_directions<
                king_jump_directions
        >::type king_turn_directions;

        typedef typename turn_directions<
                pawn_jump_directions
        >::type pawn_turn_directions;

        typedef typename boost::mpl::or_<
                std::is_same<pawn_jump_directions, directions::down>,
                boost::mpl::and_<
                        std::is_same<pawn_jump_directions, directions::up>,
                        std::is_same<pawn_promotion, promotion::apres_fini>
                >
        >::type is_unambiguous_pawn_jump;

        typedef typename boost::mpl::eval_if<
                boost::mpl::or_<
                        boost::mpl::and_<
                                std::is_same<jump_removal, removal::apres_fini>,
                                std::is_same<is_jump_direction_reversal, boost::mpl::true_>
                        >,
                        std::is_same<pawn_jump_directions, directions::all>
                >,
                boost::mpl::int_<3>,
                boost::mpl::int_<4>
        >::type large_jump;

        typedef typename boost::mpl::not_<
                std::is_same<jump_precedence, precedence::none>
        >::type is_precedence;

        // TODO: use C++11 template aliases
        template<typename Board>
        struct capture_value_type
        :
                capture::Value<rules_tag, Board>
        {};
};

}       // namespace rules
}       // namespace dctl
