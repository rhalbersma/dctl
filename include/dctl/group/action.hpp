#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/lambda.hpp>         // lambda, _1, _2
#include <boost/mpl/logical.hpp>        // and_, not_, true_
#include <dctl/group/primitives.hpp>    // set, plus, identity, minus

namespace dctl {
namespace group {
namespace action {
namespace detail {

template<class X, class Op, class A, class B>
struct is_associativity_primitive
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

template<class X, class G, class A, class B>
struct is_associativity_pair
:
        is_associativity_primitive< X, typename plus<G>::type, A, B >
{};

template<class X, class G, class A>
struct is_associativity_element
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                is_associativity_pair< X, G, A, boost::mpl::_2 >
                        >
                >
        >
{};

template<class X, class Op, class E>
struct is_identity_primitive
:
        std::is_same< typename boost::mpl::apply< Op, X, E >::type, X >
{};

}       // namespace right_action

template<class X, class G>
struct is_associativity
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                detail::is_associativity_element< X, G, boost::mpl::_2 >
                        >
                >
        >
{};

template<class X, class G>
struct is_identity
:
        detail::is_identity_primitive<
                X, typename
                plus<G>::type, typename
                identity<G>::type
        >
{};

template<class X, class G>
struct is_realized
:
        boost::mpl::and_<
                is_associativity< X, G >,
                is_identity< X, G >
        >
{};

}       // namespace action
}       // namespace group
}       // namespace dctl
