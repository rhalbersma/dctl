#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/at.hpp>             // at
#include <boost/mpl/comparison.hpp>     // and_
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // less, less_equal
#include "Angle.hpp"
#include "Dimensions.hpp"
#include "Transform.hpp"
#include "Coordinates.hpp"
#include "Grid.hpp"
#include "Traits.hpp"

namespace dctl {
namespace board {

template<typename Board, typename SQ>
struct is_square
:
        boost::mpl::and_<
                boost::mpl::less_equal<
                        boost::mpl::int_<0>,
                        SQ 
                >,
                boost::mpl::less<
                        SQ,
                        boost::mpl::int_<Board::ExternalGrid::size>
                >
        >
{};

template<typename Board, typename Color, typename SQ>
struct is_initial
{
private:
        BOOST_STATIC_CONSTANT(auto, row_min = 
                Color::value? (Board::height - 1) - ((Board::height - Board::dmz) / 2 - 1) : 0
        );        
        BOOST_STATIC_CONSTANT(auto, row_max = 
                Color::value? (Board::height - 1) : (Board::height - Board::dmz) / 2 - 1
        );
        BOOST_STATIC_CONSTANT(auto, row = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ::value> >::type::row)
        );

public:
        BOOST_STATIC_CONSTANT(auto, value = (row_min <= row) && (row <= row_max));
};

template<typename Board, typename Color, typename Row, typename SQ>
struct is_row_mask
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ::value> >::type::row == 
                (Color::value? (Board::height - 1) - Row::value : Row::value))
        );
};

template<typename Board, typename Color, typename Column, typename SQ>
struct is_col_mask
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ::value> >::type::col == 
                (Color::value? (Board::width - 1) - Column::value : Column::value))
        );
};

template<typename Board, typename Group, typename SQ>
struct is_jump_group
{
private:
        typedef typename Board::ExternalGrid Grid;

        BOOST_STATIC_CONSTANT(auto, from_sq = Group::value);
        BOOST_STATIC_CONSTANT(auto, dest_sq = SQ::value);

        BOOST_STATIC_CONSTANT(auto, from_row = (Square2Coordinates< Square<Grid, from_sq> >::type::row));
        BOOST_STATIC_CONSTANT(auto, dest_row = (Square2Coordinates< Square<Grid, dest_sq> >::type::row));
        BOOST_STATIC_CONSTANT(auto, from_col = (Square2Coordinates< Square<Grid, from_sq> >::type::col));
        BOOST_STATIC_CONSTANT(auto, dest_col = (Square2Coordinates< Square<Grid, dest_sq> >::type::col));

        BOOST_STATIC_CONSTANT(auto, R1 = (from_row - dest_row) % 4);
        BOOST_STATIC_CONSTANT(auto, C1 = (from_col - dest_col) % 4);
        BOOST_STATIC_CONSTANT(auto, R2 = (R1 + 2) % 4);
        BOOST_STATIC_CONSTANT(auto, C2 = (C1 + 2) % 4);

public:
        // a diagonal or orthogonal man capture between square <FROM> and square <DEST> is possible if 
        // BOTH row AND column numbers difference == 0 mod 4 (even number of captures) OR
        // BOTH row AND column numbers difference == 2 mod 4 (odd number of captures)
        BOOST_STATIC_CONSTANT(auto, value = (!R1 && !C1) || (!R2 && !C2));
};

template<typename Board, typename Index, typename SQ>
struct is_jump_start
{
private:
        typedef typename Board::ExternalGrid Grid;        

        BOOST_STATIC_CONSTANT(auto, offset = is_diagonal<Index>::value? 2 : 4);
        
        BOOST_STATIC_CONSTANT(auto, row_min = is_up<Index>::value? offset : 0);
        BOOST_STATIC_CONSTANT(auto, row_max = (Board::height - 1) - (is_down<Index>::value? offset : 0));
        BOOST_STATIC_CONSTANT(auto, col_min = is_left<Index>::value? offset : 0);
        BOOST_STATIC_CONSTANT(auto, col_max = (Board::width - 1) - (is_right<Index>::value? offset : 0));

        BOOST_STATIC_CONSTANT(auto, row = (Square2Coordinates< Square<Grid, SQ::value> >::type::row));
        BOOST_STATIC_CONSTANT(auto, col = (Square2Coordinates< Square<Grid, SQ::value> >::type::col));

public:
        // a jump in direction <Index> is possible if square <SQ> is within offset of the edges being approached
        BOOST_STATIC_CONSTANT(auto, value =
	        (row_min <= row) && (row <= row_max) &&
		(col_min <= col) && (col <= col_max)
	);
};

template<typename Board, int SQ>
struct square_to_bit
{
private:
        typedef typename Board::ExternalGrid E;
        typedef typename Board::InternalGrid I;

        // square coordinates within the external grid
        typedef typename Square2Coordinates< Square<E, SQ> >::type External;

        // rotated coordinates within the external grid
        typedef typename rotate<External, typename Board::full_angle >::type rotated;

public:
        // bit coordintaes re-interpreted within the internal grid
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Coordinates2Square< Coordinates<I, rotated::row, rotated::col> >::type::value)
        );
};

template<typename Board, int B>
struct bit_to_square
{
private:
        typedef typename Board::InternalGrid I;
        typedef typename Board::ExternalGrid E;

        // bit coordinates within the internal grid
        typedef typename Square2Coordinates< Square<I, B> >::type Internal;

        // rotated coordinates within the external grid
        typedef typename rotate<Internal, typename Board::inverse_angle >::type rotated;

public:
        // square coordinates re-interpreted within the internal grid
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Coordinates2Square< Coordinates<E, rotated::row, rotated::col> >::type::value)
        );
};

}       // namespace board
}       // namespace dctl
