#pragma once
#include <boost/mpl/apply.hpp>                  // apply
#include <boost/mpl/bitwise.hpp>                // bitxor_, shift_left
#include <boost/mpl/eval_if.hpp>                // eval_if_
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/integral_c.hpp>             // integral_c
#include <boost/mpl/next_prior.hpp>             // prior
#include <boost/mpl/placeholders.hpp>           // _1
#include <dctl/board/mask/init_fwd.hpp>         // init, detail::init, detail::test (primary template and partial specialization declarations)
#include <dctl/board/coordinates/convert.hpp>   // square2bit
#include <dctl/utility/int.hpp>                 // BitBoard

namespace dctl {
namespace board {
namespace mask {

// partial specialization definitions
template
<
        template<class, class, class...> class Predicate,
        class Board,
        class... Args
>
struct init< Predicate<Board, boost::mpl::_1, Args...> >
:
        detail::init< Board, Predicate<Board, boost::mpl::_1, Args...> >
{};

namespace detail {

// primary template definition
template<class Board, class Predicate, class Square>
struct init
:
        boost::mpl::bitxor_<
                init< Board, Predicate, typename boost::mpl::prior<Square>::type >,
                test< Board, Predicate, Square >
        >
{};

// partial specialization definition
template<class Board, class Predicate>
struct init<Board, Predicate, boost::mpl::int_<0> >
:
        test< Board, Predicate, boost::mpl::int_<0> >
{};

template<class Board, class Predicate, class Square>
struct test
:
        boost::mpl::eval_if< typename
                boost::mpl::apply< Predicate, Square >::type,
                boost::mpl::shift_left<
                        boost::mpl::integral_c<BitBoard, 1>,
                        boost::mpl::int_<square_to_bit< Board, Square::value >::type::value>
                >,
                boost::mpl::integral_c<BitBoard, 0>
        >::type
{};

}       // namespace detail
}       // namespace mask
}       // namespace board
}       // namespace dctl
