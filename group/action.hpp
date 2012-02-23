#pragma once
#include "primitives.hpp"
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/lambda.hpp>         // lambda, _1, _2
#include <boost/mpl/logical.hpp>        // and_, not_, true_

namespace dctl {
namespace group {

template<typename X, typename Op, typename A, typename B>
struct is_right_associativity
:
        std::is_same< typename 
                boost::mpl::apply< 
                        Op, X, typename boost::mpl::apply< Op, A, B >::type
                >::type, typename 
                boost::mpl::apply<
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
        boost::mpl::fold< typename 
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1, 
                                is_right_associativity_pair< X, G, A, boost::mpl::_2 >
                        >
                >
        >
{};

template<typename X, typename G>
struct is_right_associativity_action
:
        boost::mpl::fold< typename 
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1, 
                                is_right_associativity_element< X, G, boost::mpl::_2 >
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
                X, typename 
                plus<G>::type, typename 
                identity<G>::type
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

}       // namespace group
}       // namespace dctl
