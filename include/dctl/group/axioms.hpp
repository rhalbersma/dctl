#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/contains.hpp>       // contains
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/lambda.hpp>         // lambda
#include <boost/mpl/logical.hpp>        // and_, not_, true_
#include <boost/mpl/placeholders.hpp>   // _1, _2
#include <dctl/group/primitives.hpp>    // set, plus, identity, minus

namespace dctl {
namespace group {
namespace axioms {
namespace detail {

template<class S, class Op, class A, class B>
struct is_closure_primitive
:
        boost::mpl::contains<
                S, typename
                boost::mpl::apply< Op, A, B >::type
        >
{};

template<class G, class A, class B>
struct is_closure_pair
:
        is_closure_primitive< typename
                set<G>::type, typename
                plus<G>::type,
                A, B
        >
{};

template<class G, class A>
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

template<class Op, class A, class B, class C>
struct is_associativity_primitive
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

template<class G, class A, class B, class C>
struct is_associativity_triplet
:
        is_associativity_primitive< typename
                plus<G>::type, A, B, C
        >
{};

template<class G, class A, class B>
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

template<class G, class A>
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

template<class S, class Op, class E, class A>
struct is_identity_primitive
:
        boost::mpl::and_<
                boost::mpl::contains< S, E >,
                std::is_same<typename boost::mpl::apply< Op, A, E >::type, A>,
                std::is_same<typename boost::mpl::apply< Op, E, A >::type, A>
        >
{};

template<class G, class A>
struct is_identity_element
:
        is_identity_primitive< typename
                set<G>::type, typename
                plus<G>::type, typename
                identity<G>::type,
                A
        >
{};

template<class S, class Op, class E, class I, class A>
struct is_inverse_primitive
:
        boost::mpl::and_<
                boost::mpl::contains< S, I >,
                std::is_same<typename boost::mpl::apply< Op, A, I >::type, E>,
                std::is_same<typename boost::mpl::apply< Op, I, A >::type, E>
        >
{};

template<class G, class A>
struct is_inverse_element
:
        is_inverse_primitive< typename
                set<G>::type, typename
                plus<G>::type, typename
                identity<G>::type, typename
                boost::mpl::apply< typename
                        minus<G>::type, A
                >::type,
                A
        >
{};

}       // namespace detail 

template<class G>
struct is_closure
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                detail::is_closure_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<class G>
struct is_associativity
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                detail::is_associativity_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<class G>
struct is_identity
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                detail::is_identity_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<class G>
struct is_inverse
:
        boost::mpl::fold< typename
                set<G>::type,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                detail::is_inverse_element< G, boost::mpl::_2 >
                        >
                >
        >
{};

template<class G>
struct is_realized
:
        boost::mpl::and_<
                is_closure<G>,
                is_associativity<G>,
                is_identity<G>,
                is_inverse<G>
        >
{};

}       // namespace axioms
}       // namespace group
}       // namespace dctl
