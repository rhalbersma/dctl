#pragma once
#include <boost/mpl/arithmetic.hpp>     // minus, modulus, plus
#include <boost/mpl/comparison.hpp>     // equal_to
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <boost/mpl/placeholders.hpp>   // _1
#include <dctl/angle/traits.hpp>
#include <dctl/board/coordinates.hpp>
#include <dctl/board/coordinates/transform.hpp>
#include <dctl/board/dimensions.hpp>
#include <dctl/board/grid.hpp>
#include <dctl/mpl/type_traits.hpp>

namespace dctl {
namespace board {
namespace mask {

template<class Board, class SQ>
struct is_square
:
        mpl::is_within_range<
                SQ,
                boost::mpl::int_<0>,
                boost::mpl::int_<Board::ExternalGrid::size>
        >
{};

template<class Board, class SQ, class Color, class Separation>
struct is_initial
:
        mpl::is_within_range< typename
                Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus<
                                boost::mpl::int_<Board::height>,
                                boost::mpl::divides<
                                        boost::mpl::minus<
                                                boost::mpl::int_<Board::height>,
                                                Separation
                                        >,
                                        boost::mpl::int_<2>
                                >
                        >,
                        boost::mpl::int_<0>
                >::type, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::int_<Board::height>,
                        boost::mpl::divides<
                                boost::mpl::minus<
                                        boost::mpl::int_<Board::height>,
                                        Separation
                                >,
                                boost::mpl::int_<2>
                        >
                >::type
        >
{};

template<class Board, class SQ, class Color, class Row>
struct is_row
:
        boost::mpl::equal_to< typename
                Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus<
                                boost::mpl::int_<Board::height>,
                                boost::mpl::int_<1>,
                                Row
                        >,
                        Row
                >::type
        >
{};

template<class Board, class SQ, class Color, class Column>
struct is_col
:
        boost::mpl::equal_to< typename
                Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::col, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus<
                                boost::mpl::int_<Board::width>,
                                boost::mpl::int_<1>,
                                Column
                        >,
                        Column
                >::type
        >
{};

namespace detail {

template<class R0, class C0, class R1, class C1>
struct is_even_or_odd_jump_difference
:
        // a diagonal or orthogonal man jump between square <FROM> and square <DEST> is possible if
        boost::mpl::or_<
                // either both row and column numbers difference == 0 mod 4 (even number of jumps)
                boost::mpl::and_<
                        boost::mpl::not_<R0>, boost::mpl::not_<C0>
                >,
                // or both row and column numbers difference == 2 mod 4 (odd number of jumps)
                boost::mpl::and_<
                        boost::mpl::not_<R1>, boost::mpl::not_<C1>
                >
        >
{};

template<class R, class C>
struct is_jump_difference
:
        is_even_or_odd_jump_difference<
                boost::mpl::modulus< R, boost::mpl::int_<4> >,
                boost::mpl::modulus< C, boost::mpl::int_<4> >,
                boost::mpl::modulus<
                        boost::mpl::plus< R, boost::mpl::int_<2> >,
                        boost::mpl::int_<4>
                >,
                boost::mpl::modulus<
                        boost::mpl::plus< C, boost::mpl::int_<2> >,
                        boost::mpl::int_<4>
                >
        >
{};

template<class From, class Dest>
struct is_jump_connected
:
        is_jump_difference<
                boost::mpl::minus< typename
                        From::row, typename
                        Dest::row
                >,
                boost::mpl::minus< typename
                        From::col, typename
                        Dest::col
                >
        >
{};

template<class Grid, class FromSq, class DestSq>
struct is_jump_group
:
        is_jump_connected< typename
                Square2Coordinates< Square<Grid, FromSq> >::type, typename
                Square2Coordinates< Square<Grid, DestSq> >::type
        >
{};

}       // namespace detail

template<class Board, class SQ, class Group>
struct is_jump_group
:
        detail::is_jump_group< typename
                Board::ExternalGrid,
                Group,
                SQ
        >
{};

namespace detail {

template<class Board, class Direction, class SQ, class Grid, class Offset>
struct is_jump_start
:
        boost::mpl::and_<
                // row_min <= row < row_max
                mpl::is_within_range< typename
                        Square2Coordinates< Square<Grid, SQ> >::type::row, typename
                        boost::mpl::eval_if< 
                                angle::lazy::is_up<Direction>, 
                                Offset, 
                                boost::mpl::int_<0> 
                        >::type,
                        boost::mpl::minus<
                                boost::mpl::int_<Board::height>, typename
                                boost::mpl::eval_if< 
                                        angle::lazy::is_down<Direction>, 
                                        Offset, 
                                        boost::mpl::int_<0> 
                                >::type
                        >
                >,
                // col_min <= col < col_max
                mpl::is_within_range< typename
                        Square2Coordinates< Square<Grid, SQ> >::type::col, typename
                        boost::mpl::eval_if< 
                                angle::lazy::is_left<Direction>, 
                                Offset, 
                                boost::mpl::int_<0> 
                        >::type,
                        boost::mpl::minus<
                                boost::mpl::int_<Board::width>, typename
                                boost::mpl::eval_if< 
                                        angle::lazy::is_right<Direction>, 
                                        Offset, 
                                        boost::mpl::int_<0> 
                                >::type
                        >
                >
        >
{};

}       // namespace detail

template<class Board, class SQ, class Direction>
struct is_jump_start
:
        detail::is_jump_start<
                Board, Direction, SQ, typename
                Board::ExternalGrid, typename
                boost::mpl::eval_if<
                        angle::lazy::is_diagonal<Direction>,
                        boost::mpl::int_<2>,
                        boost::mpl::int_<4>
                >::type
        >
{};

}       // namespace mask

namespace detail {

template<class FromGrid, class DestGrid, class Angle, class N>
struct transform
:
        Coordinates2Square<
                Coordinates<
                        DestGrid,
                        mpl::lazy::rotate<
                                Square2Coordinates< Square<FromGrid, N> >,
                                Angle
                        >::type::row::value,
                        mpl::lazy::rotate<
                                Square2Coordinates< Square<FromGrid, N> >,
                                Angle
                        >::type::col::value
                >
        >
{};

}       // namespace detail

template<class Board, class Number>
struct square_to_bit
:
        detail::transform< typename
                Board::ExternalGrid, typename
                Board::InternalGrid,
                angle::Degrees<Board::orientation>,
                Number
        >
{};

template<class Board, class Number>
struct bit_to_square
:
        detail::transform< typename
                Board::InternalGrid, typename
                Board::ExternalGrid,
                mpl::lazy::inverse< angle::Degrees<Board::orientation> >,
                Number
        >
{};

}       // namespace board
}       // namespace dctl
