#pragma once
#include <boost/test/unit_test.hpp>             // BOOST_CHECK
#include <type_traits>                          // is_same
#include <boost/mpl/apply.hpp>                  // apply
#include <boost/mpl/begin_end.hpp>              // end
#include <boost/mpl/comparison.hpp>             // equal_to
#include <boost/mpl/contains.hpp>               // contains
#include <boost/mpl/deref.hpp>                  // deref
#include <boost/mpl/find_if.hpp>                // find_if
#include <boost/mpl/for_each.hpp>               // for_each
#include <boost/mpl/identity.hpp>               // identity, make_identity
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/logical.hpp>                // not_
#include <boost/mpl/placeholders.hpp>           // _1
#include "../../src/board/Transform.hpp"        // rotate

namespace dctl {

template<typename G>
struct check_axioms
{
        void operator()()
        {
                check_closure< G >()();
                check_associativity< G >()();
                check_identity< G >()();
                check_inverse< G >()();
        }
};

template<typename G>
struct check_closure
{
        void operator()()
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_element_closure<G>()
                );
        }
};

template<typename G>
struct check_element_closure
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_pair_closure<G, A>()
                );
        }
};

template<typename G, typename A>
struct check_pair_closure
{
        template<typename B>
        void operator()(boost::mpl::identity<B>)
        {
                BOOST_CHECK((boost::mpl::contains<G, typename rotate<A, B>::type>::value));
        }
};

template<typename G>
struct check_associativity
{
        void operator()()
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_element_associativity<G>()
                );
        }
};

template<typename G>
struct check_element_associativity
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_pair_associativity<G, A>()
                );
        }
};

template<typename G, typename A>
struct check_pair_associativity
{
        template<typename B>
        void operator()(boost::mpl::identity<B>)
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_triplet_associativity<A, B>()
                );
        }
};

template<typename A, typename B>
struct check_triplet_associativity
{
        template<typename C>
        void operator()(boost::mpl::identity<C>)
        {
                BOOST_CHECK((
                        std::is_same<
                                typename rotate< A, rotate< B, C > >::type,
                                typename rotate< rotate< A, B >, C >::type
                        >::value                        
                ));                
        }
};

template<typename G>
struct check_identity
{
        void operator()()
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_identity_element<G>()
                );
        }
};

template<typename G>
struct check_identity_element
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename group_identity<G>::type E;

                BOOST_CHECK((boost::mpl::contains<G, E>::value));
                BOOST_CHECK((std::is_same<typename rotate< A, E >::type, A>::value));
                BOOST_CHECK((std::is_same<typename rotate< E, A >::type, A>::value));
        }
};

template<typename G>
struct check_inverse
{
        void operator()()
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_inverse_element<G>()
                );
        }
};

template<typename G>
struct check_inverse_element
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename inverse<A>::type I;
                typedef typename group_identity<G>::type E;

                BOOST_CHECK((boost::mpl::contains<G, I>::value));
                BOOST_CHECK((std::is_same<typename rotate< A, I >::type, E>::value));
                BOOST_CHECK((std::is_same<typename rotate< I, A >::type, E>::value));
        }
};

template<typename X, typename G>
struct check_right_action
{
        void operator()()
        {
                check_right_associativity< X, G >()();
                check_right_identity< X, G >()();
        }
};

template<typename X, typename G>
struct check_right_associativity
{
        void operator()()
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_element_right_associativity<X, G>()
                );
        }
};

template<typename X, typename G>
struct check_element_right_associativity
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                boost::mpl::for_each<G, boost::mpl::make_identity<> >(
                        check_pair_right_associativity<X, A>()
                );
        }
};

template<typename X, typename A>
struct check_pair_right_associativity
{
        template<typename B>
        void operator()(boost::mpl::identity<B>)
        {
                BOOST_CHECK((
                        std::is_same<
                                typename rotate< X, rotate< A, B > >::type,
                                typename rotate< rotate< X, A >, B >::type
                        >::value                        
                ));
        }
};

template<typename X, typename G>
struct check_right_identity
{
        void operator()()
        {
                BOOST_CHECK((
                        std::is_same<
                                typename rotate< X, group_identity<G> >::type,
                                X
                        >::value                        
                ));
        }
};

template<typename G>
struct group_identity
:
        boost::mpl::deref<
                typename boost::mpl::find_if<
                        G,
                        boost::mpl::equal_to<
                                boost::mpl::placeholders::_1,
                                boost::mpl::int_<0>
                        >
                >::type
        >::type
{};

template<typename F, typename X>
struct is_idempotent
:
        std::is_same<
                typename boost::mpl::apply< F, typename boost::mpl::apply< F, X >::type >::type,
                X
        >              
{};

}       // namespace dctl
