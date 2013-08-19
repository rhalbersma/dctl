#pragma once
#include <boost/mpl/arithmetic.hpp>     // minus, modulus, plus
#include <boost/mpl/comparison.hpp>     // equal_to
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <boost/mpl/placeholders.hpp>   // _1
#include <dctl/angle/traits.hpp>
#include <dctl/angle/transform.hpp>
#include <dctl/board/coordinates.hpp>
#include <dctl/board/coordinates/transform.hpp>
#include <dctl/board/dimensions.hpp>
#include <dctl/board/grid.hpp>
#include <dctl/mpl/type_traits.hpp>

namespace dctl {
namespace board {
namespace mask {

template<class Grid, class SQ>
struct is_square
:
        mpl::is_within_range<
                boost::mpl::int_<SQ::value>,
                boost::mpl::int_<0>,
                boost::mpl::int_<Grid::size>
        >
{};

template<class Grid, class SQ, class Color, class Separation>
struct is_initial
:
        mpl::is_within_range<
                boost::mpl::int_< board::detail::decentralize(Square2Coordinates< Square<Grid, SQ::value> >::type::row, Grid::height) >, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus<
                                boost::mpl::int_<Grid::height>,
                                boost::mpl::divides<
                                        boost::mpl::minus<
                                                boost::mpl::int_<Grid::height>,
                                                Separation
                                        >,
                                        boost::mpl::int_<2>
                                >
                        >,
                        boost::mpl::int_<0>
                >::type, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::int_<Grid::height>,
                        boost::mpl::divides<
                                boost::mpl::minus<
                                        boost::mpl::int_<Grid::height>,
                                        Separation
                                >,
                                boost::mpl::int_<2>
                        >
                >::type
        >
{};

template<class Grid, class SQ, class Color, class Row>
struct is_row
:
        boost::mpl::equal_to<
                boost::mpl::int_< board::detail::decentralize(Square2Coordinates< Square<Grid, SQ::value> >::type::row, Grid::height) >, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus<
                                boost::mpl::int_<Grid::height>,
                                boost::mpl::int_<1>,
                                Row
                        >,
                        Row
                >::type
        >
{};

template<class Grid, class SQ, class Color, class Column>
struct is_col
:
        boost::mpl::equal_to<
                boost::mpl::int_< board::detail::decentralize(Square2Coordinates< Square<Grid, SQ::value> >::type::col, Grid::width) >, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus<
                                boost::mpl::int_<Grid::width>,
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

template<class FromCoord, class DestCoord>
struct is_jump_connected
:
        is_jump_difference<
                boost::mpl::minus<
                        boost::mpl::int_<FromCoord::row>,
                        boost::mpl::int_<DestCoord::row>
                >,
                boost::mpl::minus<
                        boost::mpl::int_<FromCoord::col>,
                        boost::mpl::int_<DestCoord::col>
                >
        >
{};

template<class Grid, class FromSq, class DestSq>
struct is_jump_group
:
        is_jump_connected< typename
                Square2Coordinates< Square<Grid, FromSq::value> >::type, typename
                Square2Coordinates< Square<Grid, DestSq::value> >::type
        >
{};

}       // namespace detail

template<class Grid, class SQ, class Group>
struct is_jump_group
:
        detail::is_jump_group<
                Grid,
                Group,
                SQ
        >
{};

namespace detail {

template<class Grid, class SQ, int Direction, class Offset>
struct is_jump_start
:
        boost::mpl::and_<
                // row_min <= row < row_max
                mpl::is_within_range<
                        boost::mpl::int_< board::detail::decentralize(Square2Coordinates< Square<Grid, SQ::value> >::type::row, Grid::height) >, typename
                        boost::mpl::eval_if< 
                                boost::mpl::bool_< angle::is_up(Direction) >,
                                Offset, 
                                boost::mpl::int_<0> 
                        >::type,
                        boost::mpl::minus<
                                boost::mpl::int_<Grid::height>, typename
                                boost::mpl::eval_if< 
                                        boost::mpl::bool_< angle::is_down(Direction) >,
                                        Offset, 
                                        boost::mpl::int_<0> 
                                >::type
                        >
                >,
                // col_min <= col < col_max
                mpl::is_within_range<
                        boost::mpl::int_< board::detail::decentralize(Square2Coordinates< Square<Grid, SQ::value> >::type::col, Grid::width) >, typename
                        boost::mpl::eval_if< 
                                boost::mpl::bool_< angle::is_left(Direction) >,
                                Offset, 
                                boost::mpl::int_<0> 
                        >::type,
                        boost::mpl::minus<
                                boost::mpl::int_<Grid::width>, typename
                                boost::mpl::eval_if< 
                                        boost::mpl::bool_< angle::is_right(Direction) >,
                                        Offset, 
                                        boost::mpl::int_<0> 
                                >::type
                        >
                >
        >
{};

}       // namespace detail

template<class Grid, class SQ, class Direction>
struct is_jump_start
:
        detail::is_jump_start<
                Grid, SQ, Direction::value, typename
                boost::mpl::eval_if<
                        boost::mpl::bool_< angle::is_diagonal(Direction::value) >,
                        boost::mpl::int_<2>,
                        boost::mpl::int_<4>
                >::type
        >
{};

}       // namespace mask
}       // namespace board
}       // namespace dctl
