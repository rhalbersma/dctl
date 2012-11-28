#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/placeholders.hpp>   // _1

namespace dctl {
namespace board {
namespace mask {

// primary template declaration
template<typename>
struct init;

#ifndef _MSC_VER

// partial specialization declarations
template
<
        template<typename, typename, typename...> class Predicate,
        typename Board,
        typename... Args
>
struct init< Predicate<Board, boost::mpl::_1, Args...> >;

#else

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
struct init< UnaryPredicate<Board, boost::mpl::_1, Arg1> >;

// partial specialization declaration for binary predicates
template
<
        template<typename, typename, typename, typename> class BinaryPredicate,
        typename Board, 
        typename Arg1, 
        typename Arg2
>
struct init< BinaryPredicate<Board, boost::mpl::_1, Arg1, Arg2> >;

#endif

namespace detail {

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
