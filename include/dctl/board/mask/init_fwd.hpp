#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/placeholders.hpp>   // _1

namespace dctl {
namespace board {
namespace mask {

// primary template declaration
template
<
        template<class, class, class...> class Predicate,
        class Board,
        class... Args
>
struct init;

namespace detail {

// primary template declaration
template
<
        template<class, class, class...> class Predicate,
        class Board,
        class Square,
        class... Args
>
struct init;

// partial specialization declaration
template
<
        template<class, class, class...> class Predicate,
        class Board,
        class... Args
>
struct init<Predicate, Board, boost::mpl::int_<0>, Args...>;

template
<
        class, class, class
>
struct test;

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
