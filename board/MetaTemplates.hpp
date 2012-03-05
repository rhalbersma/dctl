#pragma once
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/bitxor.hpp>         // bitxor_
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/eval_if.hpp>        // eval_if_
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/integral_c.hpp>     // integral_c
#include <boost/mpl/prior.hpp>          // prior
#include <boost/mpl/shift_left.hpp>     // shift_left
#include "Predicates.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace board {

template<typename Board, typename Predicate, typename Square>
struct Test
:
        boost::mpl::eval_if< typename 
                boost::mpl::apply< Predicate, Square >::type,
                boost::mpl::shift_left<
                        boost::mpl::integral_c<BitBoard, 1>,
                        boost::mpl::int_< square_to_bit< Board, Square::value >::type::value >
                >,
                boost::mpl::integral_c<BitBoard, 0>
        >
{};

template
<
        typename Board,
        typename Predicate, 
        typename Square = boost::mpl::int_<Board::ExternalGrid::size - 1>
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

template<typename Board, int Group>
struct init_jump_group
:
        Init< Board, is_jump_group< Board, boost::mpl::int_<Group>, boost::mpl::_1 > >
{};

template<typename Board, typename Index>
struct init_jump_start
:
        Init< Board, is_jump_start< Board, Index, boost::mpl::_1 > >
{};

}       // namespace board
}       // namespace dctl
