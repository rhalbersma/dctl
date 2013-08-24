#pragma once
#include <boost/mpl/arithmetic.hpp>     // minus, modulus, plus
#include <boost/mpl/comparison.hpp>     // equal_to
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // and_, not_, or_
#include <boost/mpl/placeholders.hpp>   // _1
#include <dctl/angle/traits.hpp>
#include <dctl/angle/transform.hpp>
#include <dctl/grid/coordinates.hpp>
#include <dctl/grid/coordinates/transform.hpp>
#include <dctl/grid/dimensions.hpp>
#include <dctl/grid/grid.hpp>
#include <dctl/mpl/type_traits.hpp>

namespace dctl {
namespace grid {

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
                boost::mpl::int_< grid::detail::decentralize( grid::sqtocoord( Square<Grid>(SQ::value) ).row(), Grid::height) >, typename
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
                boost::mpl::int_< grid::detail::decentralize( grid::sqtocoord( Square<Grid>(SQ::value) ).row(), Grid::height) >, typename
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
                boost::mpl::int_< grid::detail::decentralize( grid::sqtocoord( Square<Grid>(SQ::value) ).col(), Grid::width) >, typename
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

constexpr auto is_even_or_odd_jump_difference(int R0, int C0, int R1, int C1) noexcept
{
        // a diagonal or orthogonal man jump between square <FROM> and square <DEST> is possible if
        // either both row and column numbers difference == 0 mod 4 (even number of jumps)
        // or both row and column numbers difference == 2 mod 4 (odd number of jumps)
        return (!R0 && !C0) || (!R1 && !C1);
}

constexpr auto is_jump_difference(int delta_row, int delta_col) noexcept
{
        return is_even_or_odd_jump_difference(
                delta_row % 4,
                delta_col % 4,
                (delta_row + 2) % 4,
                (delta_col + 2) % 4
        );
}

template<class Coordinates>
constexpr auto is_jump_connected(Coordinates const& from_coord, Coordinates const& dest_coord) noexcept
{
        return is_jump_difference(
                from_coord.row() - dest_coord.row(),
                from_coord.col() - dest_coord.col()
        );
}

template<class Square>
constexpr auto is_jump_group(Square const& from_sq, Square const& dest_sq) noexcept
{
        return is_jump_connected(
                sqtocoord( Square{from_sq.value()} ),
                sqtocoord( Square{dest_sq.value()} )
        );
}

}       // namespace detail

template<class Square>
constexpr auto is_jump_group(int group, int square) noexcept
{
        return detail::is_jump_group( Square{group}, Square{square} );
}

namespace detail {

template<class Grid, class SQ, int Direction, class Offset>
struct is_jump_start
:
        boost::mpl::and_<
                // row_min <= row < row_max
                mpl::is_within_range<
                        boost::mpl::int_< grid::detail::decentralize( grid::sqtocoord( Square<Grid>(SQ::value) ).row(), Grid::height) >, typename
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
                        boost::mpl::int_< grid::detail::decentralize( grid::sqtocoord( Square<Grid>(SQ::value) ).col(), Grid::width) >, typename
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

}       // namespace grid
}       // namespace dctl
