#pragma once
#include <boost/mpl/arithmetic.hpp>     // minus, modulus, plus
#include <boost/mpl/comparison.hpp>     // equal_to
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <boost/mpl/placeholders.hpp>   // _1
#include <dctl/board/Dimensions.hpp>
#include <dctl/board/Coordinates.hpp>
#include <dctl/board/coordinates/transform.hpp>
#include <dctl/board/Grid.hpp>
#include <dctl/angle/traits.hpp>
#include <dctl/mpl/type_traits.hpp>

namespace dctl {
namespace board {
namespace mask {

template<typename Board, typename SQ>
struct is_square
:
        mpl::is_within_range<
                SQ,
                boost::mpl::int_<0>, typename
                Board::ExternalGrid::size
        >
{};

template<typename Board, typename SQ, typename Color, typename Separation>
struct is_initial
:
        mpl::is_within_range< typename
                Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus< typename
                                Board::height,
                                boost::mpl::divides<
                                        boost::mpl::minus< typename
                                                Board::height, 
                                                Separation
                                        >,
                                        boost::mpl::int_<2>
                                >
                        >,
                        boost::mpl::int_<0>
                >::type, typename
                boost::mpl::eval_if<
                        Color, typename
                        Board::height,
                        boost::mpl::divides<
                                boost::mpl::minus< typename
                                        Board::height,
                                        Separation
                                >,
                                boost::mpl::int_<2>
                        >
                >::type
        >
{};

template<typename Board, typename SQ, typename Color, typename Row>
struct is_row
:
        boost::mpl::equal_to< typename
                Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus< typename
                                Board::height,
                                boost::mpl::int_<1>,
                                Row
                        >,
                        Row
                >::type
        >
{};

template<typename Board, typename SQ, typename Color, typename Column>
struct is_col
:
        boost::mpl::equal_to< typename
                Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::col, typename
                boost::mpl::eval_if<
                        Color,
                        boost::mpl::minus< typename
                                Board::width,
                                boost::mpl::int_<1>,
                                Column
                        >,
                        Column
                >::type
        >
{};

namespace detail {

template<typename R0, typename C0, typename R1, typename C1>
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

template<typename R, typename C>
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

template<typename From, typename Dest>
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

template<typename Grid, typename FromSq, typename DestSq>
struct is_jump_group
:
        is_jump_connected< typename
                Square2Coordinates< Square<Grid, FromSq> >::type, typename
                Square2Coordinates< Square<Grid, DestSq> >::type
        >
{};

}       // namespace detail

template<typename Board, typename SQ, typename Group>
struct is_jump_group
:
        detail::is_jump_group< typename
                Board::ExternalGrid,
                Group,
                SQ
        >
{};

namespace detail {

template<typename Board, typename Direction, typename SQ, typename Grid, typename Offset>
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
                        boost::mpl::minus< typename
                                Board::height, typename
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
                        boost::mpl::minus< typename
                                Board::width, typename
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

template<typename Board, typename SQ, typename Direction>
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

template<typename FromGrid, typename DestGrid, typename Angle, typename N>
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

template<typename Board, typename Number>
struct square_to_bit
:
        detail::transform< typename
                Board::ExternalGrid, typename
                Board::InternalGrid, typename
                Board::full_angle,
                Number
        >
{};

template<typename Board, typename Number>
struct bit_to_square
:
        detail::transform< typename
                Board::InternalGrid, typename
                Board::ExternalGrid, typename
                Board::inverse_angle,
                Number
        >
{};

}       // namespace board
}       // namespace dctl
