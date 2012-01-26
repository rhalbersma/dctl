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
struct is_axioms
{
        void operator()()
        {
                is_closure< G >()();
                is_associativity< G >()();
                is_identity< G >()();
                is_inverse< G >()();
        }
};

template<typename G>
struct is_closure
{
        void operator()()
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_element_closure<G>()
                );
        }
};

template<typename G>
struct is_element_closure
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_pair_closure<G, A>()
                );
        }
};

template<typename G, typename A>
struct is_pair_closure
{
        template<typename B>
        void operator()(boost::mpl::identity<B>)
        {
                typedef typename group::set<G>::type S;
                typedef typename group::plus<G>::type Op;

                BOOST_CHECK((
                        boost::mpl::contains< 
                                S, typename boost::mpl::apply< Op, A, B >::type
                        >::value
                ));
        }
};

template<typename G>
struct is_associativity
{
        void operator()()
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_element_associativity<G>()
                );
        }
};

template<typename G>
struct is_element_associativity
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_pair_associativity<G, A>()
                );
        }
};

template<typename G, typename A>
struct is_pair_associativity
{
        template<typename B>
        void operator()(boost::mpl::identity<B>)
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_triplet_associativity<G, A, B>()
                );
        }
};

template<typename G, typename A, typename B>
struct is_triplet_associativity
{
        template<typename C>
        void operator()(boost::mpl::identity<C>)
        {
                typedef typename group::plus<G>::type Op;

                BOOST_CHECK((
                        std::is_same<
                                typename boost::mpl::apply<
                                        Op, A, typename boost::mpl::apply< Op, B, C >::type 
                                >::type,
                                typename boost::mpl::apply< 
                                        Op, typename boost::mpl::apply< Op, A, B >::type, C 
                                >::type
                        >::value                        
                ));                
        }
};

template<typename G>
struct is_identity
{
        void operator()()
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_identity_element<G>()
                );
        }
};

template<typename G>
struct is_identity_element
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename group::set<G>::type S;
                typedef typename group::plus<G>::type Op;
                typedef typename group::identity<G>::type E;

                BOOST_CHECK((boost::mpl::contains<S, E>::value));
                BOOST_CHECK((std::is_same<typename boost::mpl::apply< Op, A, E >::type, A>::value));
                BOOST_CHECK((std::is_same<typename boost::mpl::apply< Op, E, A >::type, A>::value));
        }
};

template<typename G>
struct is_inverse
{
        void operator()()
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_inverse_element<G>()
                );
        }
};

template<typename G>
struct is_inverse_element
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename group::set<G>::type S;
                typedef typename group::plus<G>::type Op;
                typedef typename group::identity<G>::type E;
                typedef typename inverse<A>::type I;

                BOOST_CHECK((boost::mpl::contains<S, I>::value));
                BOOST_CHECK((std::is_same<typename boost::mpl::apply< Op, A, I >::type, E>::value));
                BOOST_CHECK((std::is_same<typename boost::mpl::apply< Op, I, A >::type, E>::value));
        }
};

template<typename X, typename G>
struct is_right_action
{
        void operator()()
        {
                is_right_associativity< X, G >()();
                is_right_identity< X, G >()();
        }
};

template<typename X, typename G>
struct is_right_associativity
{
        void operator()()
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_element_right_associativity<X, G>()
                );
        }
};

template<typename X, typename G>
struct is_element_right_associativity
{
        template<typename A>
        void operator()(boost::mpl::identity<A>)
        {
                typedef typename group::set<G>::type S;

                boost::mpl::for_each<S, boost::mpl::make_identity<> >(
                        is_pair_right_associativity<X, G, A>()
                );
        }
};

template<typename X, typename G, typename A>
struct is_pair_right_associativity
{
        template<typename B>
        void operator()(boost::mpl::identity<B>)
        {
                typedef typename group::plus<G>::type Op;

                BOOST_CHECK((
                        std::is_same<
                                typename boost::mpl::apply< 
                                        Op, X, typename boost::mpl::apply< Op, A, B >::type 
                                >::type,
                                typename boost::mpl::apply<
                                        Op, typename boost::mpl::apply< Op, X, A >::type, B 
                                >::type
                        >::value                        
                ));
        }
};

template<typename X, typename G>
struct is_right_identity
{
        void operator()()
        {
                typedef typename group::plus<G>::type Op;
                typedef typename group::identity<G>::type E;

                BOOST_CHECK((
                        std::is_same<
                                typename boost::mpl::apply< Op, X, E >::type,
                                X
                        >::value                        
                ));
        }
};

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

}       // namespace dctl
