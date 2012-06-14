#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/contains.hpp>       // contains
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/lambda.hpp>         // lambda, _1, _2
#include <boost/mpl/logical.hpp>        // and_, not_, true_
#include "primitives.hpp"

namespace dctl {
namespace group {

template<typename S, typename Op, typename A, typename B>
struct is_closure
:
        boost::mpl::contains<
                S, typename
                boost::mpl::apply< Op, A, B >::type
        >
{};

template<typename G, typename A, typename B>
struct is_closure_pair
:
        is_closure< typename
                set<G>::type, typename
                plus<G>::type,
                A, B
        >
{};

template<typename G, typename A>
struct is_closure_element
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_closure_pair< G, A, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename G>
struct is_closure_axiom
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_closure_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename Op, typename A, typename B, typename C>
struct is_associativity
:
        std::is_same< typename
                boost::mpl::apply<
                        Op, A, typename boost::mpl::apply< Op, B, C >::type
                >::type, typename
                boost::mpl::apply<
                        Op, typename boost::mpl::apply< Op, A, B >::type, C
                >::type
        >
{};

template<typename G, typename A, typename B, typename C>
struct is_associativity_triplet
:
        is_associativity< typename
                plus<G>::type, A, B, C
        >
{};

template<typename G, typename A, typename B>
struct is_associativity_pair
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_associativity_triplet< G, A, B, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename G, typename A>
struct is_associativity_element
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_associativity_pair< G, A, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename G>
struct is_associativity_axiom
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_associativity_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename S, typename Op, typename E, typename A>
struct is_identity
:
        boost::mpl::and_<
                boost::mpl::contains< S, E >,
                std::is_same<typename boost::mpl::apply< Op, A, E >::type, A>,
                std::is_same<typename boost::mpl::apply< Op, E, A >::type, A>
        >
{};

template<typename G, typename A>
struct is_identity_element
:
        is_identity< typename
                set<G>::type, typename
                plus<G>::type, typename
                identity<G>::type,
                A
        >
{};

template<typename G>
struct is_identity_axiom
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_identity_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename S, typename Op, typename E, typename I, typename A>
struct is_inverse
:
        boost::mpl::and_<
                boost::mpl::contains< S, I >,
                std::is_same<typename boost::mpl::apply< Op, A, I >::type, E>,
                std::is_same<typename boost::mpl::apply< Op, I, A >::type, E>
        >
{};

template<typename G, typename A>
struct is_inverse_element
:
        is_inverse< typename
                set<G>::type, typename
                plus<G>::type, typename
                identity<G>::type, typename
                boost::mpl::apply< typename
                        minus<G>::type, A
                >::type,
                A
        >
{};

template<typename G>
struct is_inverse_axiom
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_inverse_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename G>
struct is_axioms
:
        boost::mpl::and_<
                is_closure_axiom<G>,
                is_associativity_axiom<G>,
                is_identity_axiom<G>,
                is_inverse_axiom<G>
        >
{};

}       // namespace group
}       // namespace dctl
