#pragma once
#include <boost/mpl/apply.hpp>                  // apply
#include <boost/mpl/bitwise.hpp>                // bitxor_, shift_left
#include <boost/mpl/eval_if.hpp>                // eval_if_
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/integral_c.hpp>             // integral_c
#include <boost/mpl/next_prior.hpp>             // prior
#include <boost/mpl/placeholders.hpp>           // _1
#include <dctl/board/mask/init_fwd.hpp>        // init, detail::init, detail::test (primary template and partial specialization declarations)
#include <dctl/board/mask/predicates.hpp>      // square2bit
#include <dctl/utility/IntegerTypes.hpp>        // BitBoard

namespace dctl {
namespace board {
namespace mask {

/*

// TODO: rewrite using C++11 variadic templates
template<template<typename, typename..., typename> class Pred, typename Board, typename... Args>
struct init< Pred<Board, Args..., boost::mpl::_1> >
:
        detail::init< Board, Pred<Board, Args..., boost::mpl::_1> >
{};

*/

// partial specialization definition for nullary predicates
template
<
        template<typename, typename> class NullaryPredicate,
        typename Board
>
struct init< NullaryPredicate<Board, boost::mpl::_1> >
:
        detail::init< Board, NullaryPredicate<Board, boost::mpl::_1> >
{};

// partial specialization definition for unary predicates
template
<
        template<typename, typename, typename> class UnaryPredicate,
        typename Board, 
        typename Arg1
>
struct init< UnaryPredicate<Board, Arg1, boost::mpl::_1> >
:
        detail::init< Board, UnaryPredicate<Board, Arg1, boost::mpl::_1> >
{};

// partial specialization definition for binary predicates
template
<
        template<typename, typename, typename, typename> class BinaryPredicate,
        typename Board, 
        typename Arg1, 
        typename Arg2 
>
struct init< BinaryPredicate<Board, Arg1, Arg2, boost::mpl::_1> >
:
        detail::init< Board, BinaryPredicate<Board, Arg1, Arg2, boost::mpl::_1> >
{};

namespace detail {

// primary template definition
template<typename Board, typename Predicate, typename Square>
struct init
:
        boost::mpl::bitxor_<
                init< Board, Predicate, typename boost::mpl::prior<Square>::type >,
                test< Board, Predicate, Square >
        >
{};

// partial specialization definition
template<typename Board, typename Predicate>
struct init<Board, Predicate, boost::mpl::int_<0> >
:
        test< Board, Predicate, boost::mpl::int_<0> >
{};

template<typename Board, typename Predicate, typename Square>
struct test
:
        boost::mpl::eval_if< typename
                boost::mpl::apply< Predicate, Square >::type,
                boost::mpl::shift_left<
                        boost::mpl::integral_c<BitBoard, 1>, typename
                        square_to_bit< Board, Square >::type::number
                >,
                boost::mpl::integral_c<BitBoard, 0>
        >
{};

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
