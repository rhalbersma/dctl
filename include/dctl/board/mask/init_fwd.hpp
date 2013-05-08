#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/placeholders.hpp>   // _1

namespace dctl {
namespace board {
namespace mask {

// primary template declaration
template<typename>
struct init;

// partial specialization declarations
template
<
        template<typename, typename, typename...> class Predicate,
        typename Board,
        typename... Args
>
struct init< Predicate<Board, boost::mpl::_1, Args...> >;

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
