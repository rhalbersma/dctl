#pragma once
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/bitwise.hpp>        // bitxor_, shift_left
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/eval_if.hpp>        // eval_if_
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/integral_c.hpp>     // integral_c
#include <boost/mpl/next_prior.hpp>     // prior
#include <boost/mpl/placeholders.hpp>   // _1
#include <dctl/board/Predicates.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace board {

template<typename Board, typename Predicate, typename Square>
struct Test
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

/*
        // NOTE: on Microsoft Visual C++ 2012 Express,dbrm2 we get a
        // fatal error C1202: recursive type or function dependency context too complex
        typename Square = typename boost::mpl::minus< typename
                Board::ExternalGrid::size, 
                boost::mpl::int_<1> 
        >::type
*/
template
<
        typename Board,
        typename Predicate,
        typename Square = boost::mpl::int_<Board::ExternalGrid::size::value - 1>
>
struct Init;

template<typename Board, typename Predicate>
struct Init<Board, Predicate, boost::mpl::int_<0> >
:
        Test< Board, Predicate, boost::mpl::int_<0> >
{};

template<typename Board, typename Predicate, typename Square>
struct Init
:
        boost::mpl::bitxor_<
                Init< Board, Predicate, typename boost::mpl::prior<Square>::type >,
                Test< Board, Predicate, Square >
        >
{};

/*

template<typename Board, type_1 Arg_1, ..., type_n Arg_n>
struct init_Pred
:
        Init< Board, Pred< Board, boost::mpl::type_1<Arg_1>, ...,  boost::mpl::type_n<Arg_n>, boost::mpl::_1> >
{}

*/

template<typename Board>
struct init_squares
:
        Init< Board, is_square< Board, boost::mpl::_1 > >
{};

template<typename Board, bool Color>
struct init_initial
:
        Init< Board, is_initial< Board, boost::mpl::bool_<Color>, boost::mpl::_1 > >
{};

template<typename Board, bool Color, int Row>
struct init_row_mask
:
        Init< Board, is_row_mask< Board, boost::mpl::bool_<Color>, boost::mpl::int_<Row>, boost::mpl::_1 > >
{};

template<typename Board, bool Color, int Column>
struct init_col_mask
:
        Init< Board, is_col_mask< Board, boost::mpl::bool_<Color>, boost::mpl::int_<Column>, boost::mpl::_1 > >
{};

template<typename Board, typename Group>
struct init_jump_group
:
        Init< Board, is_jump_group< Board, Group, boost::mpl::_1 > >
{};

template<typename Board, typename Direction>
struct init_jump_start
:
        Init< Board, is_jump_start< Board, Direction, boost::mpl::_1 > >
{};

}       // namespace board
}       // namespace dctl
