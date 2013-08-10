#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/placeholders.hpp>   // _1

namespace dctl {
namespace board {
namespace mask {

// primary template declaration
template<class>
struct init;

// partial specialization declarations
template
<
        template<class, class, class...> class Predicate,
        class Board,
        class... Args
>
struct init< Predicate<Board, boost::mpl::_1, Args...> >;

namespace detail {

// primary template declaration
template
<
        class Board,
        class Predicate,
        class Square = boost::mpl::int_<Board::ExternalGrid::size::value - 1>
>
struct init;

// partial specialization declaration
template<class Board, class Predicate>
struct init<Board, Predicate, boost::mpl::int_<0> >;

template<class, class, class>
struct test;

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
