#pragma once
#include <boost/test/unit_test.hpp>             // BOOST_CHECK
#include <type_traits>                          // is_same
#include <boost/mpl/apply.hpp>                  // apply
#include <boost/mpl/comparison.hpp>             // equal_to
#include <boost/mpl/deref.hpp>                  // deref
#include <boost/mpl/find_if.hpp>                // find_if
#include <boost/mpl/for_each.hpp>               // for_each
#include <boost/mpl/identity.hpp>               // identity, make_identity
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include "../../src/board/Transform.hpp"        // rotate

namespace dctl {

template<typename G>
struct check_group_axioms
{
        void operator()()
        {
                check_closure< G >()();
                check_associativity< G >()();
                check_identity< G >()();
                check_inverse< G >()();
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
                        check_all_products_right_associativity<X, G>()
                );
        }
};

template<typename X, typename Group>
struct check_all_products_right_associativity
{
        template<typename G>
        void operator()(boost::mpl::identity<G>)
        {
                boost::mpl::for_each<Group, boost::mpl::make_identity<> >(
                        check_is_right_associativity<X, G>()
                );
        }
};

template<typename X, typename G>
struct check_is_right_associativity
{
        template<typename H>
        void operator()(boost::mpl::identity<H>)
        {
                BOOST_CHECK((
                        std::is_same<
                                typename rotate< X, rotate< G, H > >::type,
                                typename rotate< rotate< X, G >, H >::type
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
                typename X::type,
                typename boost::mpl::apply< F, typename boost::mpl::apply< F, X >::type >::type
        >              
{};

}       // namespace dctl
