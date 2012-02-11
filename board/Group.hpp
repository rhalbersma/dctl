#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/at.hpp>             // at
#include <boost/mpl/contains.hpp>       // contains
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/lambda.hpp>         // lambda, _1, _2
#include <boost/mpl/logical.hpp>        // and_, not_, true_
#include <boost/mpl/vector.hpp>         // vector

namespace dctl {
namespace group {

namespace mpl = boost::mpl;

template<
        typename Set,
        typename Plus,
        typename Identity,
        typename Minus
>
struct Group
:
        mpl::vector<Set, Plus, Identity, Minus>
{};

template<typename G>
struct set
:
        mpl::at< G, mpl::int_<0> >
{};

template<typename G>
struct plus
:
        mpl::at< G, mpl::int_<1> >
{};

template<typename G>
struct identity
:
        mpl::at< G, mpl::int_<2> >
{};

template<typename G>
struct minus
:
        mpl::at< G, mpl::int_<3> >
{};

template<typename S, typename Op, typename A, typename B>
struct is_closure
:
        mpl::contains< 
                S, typename mpl::apply< Op, A, B >::type
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
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_closure_pair< G, A, mpl::_2 >
                        >
                >
        >
{};

template<typename G>
struct is_closure_axiom
:
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_closure_element< G, mpl::_2 >
                        >
                >
        >
{};

template<typename Op, typename A, typename B, typename C>
struct is_associativity
:
        std::is_same<
                typename mpl::apply<
                        Op, A, typename mpl::apply< Op, B, C >::type 
                >::type,
                typename mpl::apply< 
                        Op, typename mpl::apply< Op, A, B >::type, C 
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
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_associativity_triplet< G, A, B, mpl::_2 >
                        >
                >
        >
{};

template<typename G, typename A>
struct is_associativity_element
:
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_associativity_pair< G, A, mpl::_2 >
                        >
                >
        >
{};

template<typename G>
struct is_associativity_axiom
:
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_associativity_element< G, mpl::_2 >
                        >
                >
        >
{};

template<typename S, typename Op, typename E, typename A>
struct is_identity
:
        mpl::and_<
                mpl::contains< S, E >, 
                std::is_same<typename mpl::apply< Op, A, E >::type, A>,
                std::is_same<typename mpl::apply< Op, E, A >::type, A>
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
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_identity_element< G, mpl::_2 >
                        >
                >
        >
{};

template<typename S, typename Op, typename E, typename I, typename A>
struct is_inverse
:
        mpl::and_<
                mpl::contains< S, I >,
                std::is_same<typename mpl::apply< Op, A, I >::type, E>,
                std::is_same<typename mpl::apply< Op, I, A >::type, E>
        >
{};

template<typename G, typename A>
struct is_inverse_element
:
        is_inverse<
                typename set<G>::type,
                typename plus<G>::type,
                typename identity<G>::type,
                typename mpl::apply<
                        typename minus<G>::type, A
                >::type,
                A
        >
{};

template<typename G>
struct is_inverse_axiom
:
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_inverse_element< G, mpl::_2 >
                        >
                >
        >
{};

template<typename G>
struct is_axioms
:        
        mpl::and_<
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
                typename mpl::apply< 
                        Op, X, typename mpl::apply< Op, A, B >::type
                >::type,
                typename mpl::apply<
                        Op, typename mpl::apply< Op, X, A >::type, B 
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
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_right_associativity_pair< X, G, A, mpl::_2 >
                        >
                >
        >
{};

template<typename X, typename G>
struct is_right_associativity_action
:
        mpl::fold<
                typename set<G>::type,
                mpl::true_,
                mpl::lambda<
                        mpl::and_<
                                mpl::_1, 
                                is_right_associativity_element< X, G, mpl::_2 >
                        >
                >
        >
{};

template<typename X, typename Op, typename E>
struct is_right_identity
:
        std::is_same< typename mpl::apply< Op, X, E >::type, X >
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
        mpl::and_<
                is_right_associativity_action< X, G >,
                is_right_identity_action< X, G >
        >
{};

template<typename F, typename X>
struct is_idempotent
:
        std::is_same<
                typename mpl::apply< 
                        F, typename mpl::apply< F, X >::type 
                >::type,
                X
        >              
{};

}       // namespace group
}       // namespace dctl
