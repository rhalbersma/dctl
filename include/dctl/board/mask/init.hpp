#pragma once
#include <boost/mpl/apply.hpp>                  // apply
#include <boost/mpl/bitwise.hpp>                // bitxor_, shift_left
#include <boost/mpl/eval_if.hpp>                // eval_if_
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/integral_c.hpp>             // integral_c
#include <boost/mpl/next_prior.hpp>             // prior
#include <boost/mpl/placeholders.hpp>           // _1
#include <dctl/board/mask/init_fwd.hpp>         // init, detail::init, detail::test (primary template and partial specialization declarations)
#include <dctl/utility/int.hpp>                 // BitBoard

namespace dctl {
namespace board {
namespace mask {

// primary template definition
template
<
        template<class, class, class...> class Predicate,
        class Board,
        class... Args
>
struct init
:
        detail::init< Predicate, Board, boost::mpl::int_<Board::ExternalGrid::size - 1>, Args...>
{};

namespace detail {

// primary template definition
template
<
        template<class, class, class...> class Predicate,
        class Board, class Square, class... Args
>
struct init
:
        boost::mpl::bitxor_<
                init< Predicate, Board, typename boost::mpl::prior<Square>::type, Args... >,
                test< Board, Predicate<typename Board::ExternalGrid, boost::mpl::_1, Args...>, Square >
        >
{};

// partial specialization definition
template
<
        template<class, class, class...> class Predicate,
        class Board, class... Args
>
struct init<Predicate, Board, boost::mpl::int_<0>, Args... >
:
        test< Board, Predicate<typename Board::ExternalGrid, boost::mpl::_1, Args...>, boost::mpl::int_<0> >
{};

template<class Board, class Predicate, class Square>
struct test
:
        boost::mpl::eval_if< typename
                boost::mpl::apply< Predicate, Square >::type,
                boost::mpl::shift_left<
                        boost::mpl::integral_c<BitBoard, 1>,
                        boost::mpl::int_< Board::square2bit(Square::value) >
                >,
                boost::mpl::integral_c<BitBoard, 0>
        >::type
{};

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
