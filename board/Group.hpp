#pragma once
#include <type_traits>                          // is_same
#include <boost/mpl/apply.hpp>                  // apply
#include <boost/mpl/at.hpp>                     // at
#include <boost/mpl/comparison.hpp>             // equal_to
#include <boost/mpl/contains.hpp>               // contains
#include <boost/mpl/fold.hpp>                   // fold
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/lambda.hpp>                 // lambda
#include <boost/mpl/logical.hpp>                // not_
#include <boost/mpl/placeholders.hpp>           // _1
using namespace boost::mpl::placeholders;
#include <boost/mpl/vector.hpp>                 // vector

namespace dctl {
namespace group {

template<
        typename Set,
        typename Plus,
        typename Identity,
        typename Minus
>
struct Group
:
        boost::mpl::vector<Set, Plus, Identity, Minus>
{};

template<typename G>
struct set
:
        boost::mpl::at< G, boost::mpl::int_<0> >
{};

template<typename G>
struct plus
:
        boost::mpl::at< G, boost::mpl::int_<1> >
{};

template<typename G>
struct identity
:
        boost::mpl::at< G, boost::mpl::int_<2> >
{};

template<typename G>
struct minus
:
        boost::mpl::at< G, boost::mpl::int_<3> >
{};

template<typename S, typename Op, typename A, typename B>
struct is_closure
:
        boost::mpl::contains< 
                S, typename boost::mpl::apply< Op, A, B >::type
        >
{};

template<typename G, typename A, typename B>
struct is_closure_pair
:
        is_closure< 
                typename set<G>::type, 
                typename plus<G>::type, 
                A, B 
        >
{};

template<typename G, typename A>
struct is_closure_element
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_closure_pair< G, A, _2 >
                        >
                >
        >
{};

template<typename G>
struct is_closure_axiom
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_closure_element< G, _2 >
                        >
                >
        >
{};

template<typename Op, typename A, typename B, typename C>
struct is_associativity
:
        std::is_same<
                typename boost::mpl::apply<
                        Op, A, typename boost::mpl::apply< Op, B, C >::type 
                >::type,
                typename boost::mpl::apply< 
                        Op, typename boost::mpl::apply< Op, A, B >::type, C 
                >::type
        >
{};

template<typename G, typename A, typename B, typename C>
struct is_associativity_triplet
:        
        is_associativity<
                typename plus<G>::type, A, B, C
        >
{};

template<typename G, typename A, typename B>
struct is_associativity_pair
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_associativity_triplet< G, A, B, _2 >
                        >
                >
        >
{};

template<typename G, typename A>
struct is_associativity_element
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_associativity_pair< G, A, _2 >
                        >
                >
        >
{};

template<typename G>
struct is_associativity_axiom
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_associativity_element< G, _2 >
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
        is_identity<
                typename set<G>::type,
                typename plus<G>::type,
                typename identity<G>::type,
                A
        >
{};

template<typename G>
struct is_identity_axiom
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_identity_element< G, _2 >
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
        is_inverse<
                typename set<G>::type,
                typename plus<G>::type,
                typename identity<G>::type,
                typename boost::mpl::apply<
                        typename minus<G>::type, A
                >::type,
                A
        >
{};

template<typename G>
struct is_inverse_axiom
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_inverse_element< G, _2 >                        
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

template<typename X, typename Op, typename A, typename B>
struct is_right_associativity
:
        std::is_same<
                typename boost::mpl::apply< 
                        Op, X, typename boost::mpl::apply< Op, A, B >::type
                >::type,
                typename boost::mpl::apply<
                        Op, typename boost::mpl::apply< Op, X, A >::type, B 
                >::type
        >
{};

template<typename X, typename G, typename A, typename B>
struct is_right_associativity_pair
:
        is_right_associativity< X, typename plus<G>::type, A, B >
{};

template<typename X, typename G, typename A>
struct is_right_associativity_element
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_right_associativity_pair< X, G, A, _2 >
                        >
                >
        >
{};

template<typename X, typename G>
struct is_right_associativity_action
:
        boost::mpl::fold<
                typename set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                _1, is_right_associativity_element< X, G, _2 >
                        >
                >
        >
{};

template<typename X, typename Op, typename E>
struct is_right_identity
:
        std::is_same< typename boost::mpl::apply< Op, X, E >::type, X >
{};

template<typename X, typename G>
struct is_right_identity_action
:
        is_right_identity<
                X,
                typename plus<G>::type,
                typename identity<G>::type
        >
{};

template<typename X, typename G>
struct is_right_action
:
        boost::mpl::and_<
                is_right_associativity_action< X, G >,
                is_right_identity_action< X, G >
        >
{};

template<typename F, typename X>
struct is_idempotent
:
        std::is_same<
                typename boost::mpl::apply< 
                        F, typename boost::mpl::apply< F, X >::type 
                >::type,
                X
        >              
{};

}       // namespace group
}       // namespace dctl
