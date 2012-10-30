#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/placeholders.hpp>   // _1

namespace dctl {
namespace board {
namespace mask {

// primary template declaration
template<typename Predicate>
struct init;

/*

// TODO: rewrite using C++11 variadic templates
template<template<typename, typename..., typename> class Pred, typename Board, typename... Args>
struct init< Pred<Board, Args..., boost::mpl::_1> >;

*/

// partial specialization declaration for nullary predicates
template
<
        template<typename, typename> class NullaryPredicate, 
        typename Board 
>
struct init< NullaryPredicate<Board, boost::mpl::_1> >;

// partial specialization declaration for unary predicates
template
<
        template<typename, typename, typename> class UnaryPredicate,
        typename Board, 
        typename Arg1
>
struct init< UnaryPredicate<Board, Arg1, boost::mpl::_1> >;

// partial specialization declaration for binary predicates
template
<
        template<typename, typename, typename, typename> class BinaryPredicate,
        typename Board, 
        typename Arg1, 
        typename Arg2
>
struct init< BinaryPredicate<Board, Arg1, Arg2, boost::mpl::_1> >;

namespace detail {

/*
        // NOTE: on Microsoft Visual C++ 2012 Express, we get a fatal error C1202: 
        // recursive type or function dependency context too complex from
        typename Square = typename boost::mpl::minus< typename
                Board::ExternalGrid::size, 
                boost::mpl::int_<1> 
        >::type
*/

// primary template declaration
template
<
        typename Board,
        typename Predicate,
        typename Square = boost::mpl::int_<Board::ExternalGrid::size::value - 1>
>
struct init;

// partial specialization declaration
template<typename Board, typename Predicate>
struct init<Board, Predicate, boost::mpl::int_<0> >;

template<typename, typename, typename>
struct test;

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
