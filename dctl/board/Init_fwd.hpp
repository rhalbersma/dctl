#pragma once
#include <boost/mpl/int.hpp>            // int_

namespace dctl {
namespace board {

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
struct Init;

// partial specialization declaration
template<typename Board, typename Predicate>
struct Init<Board, Predicate, boost::mpl::int_<0> >;

}       // namespace board
}       // namespace dctl
