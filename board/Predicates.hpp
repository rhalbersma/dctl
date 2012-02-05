#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/at.hpp>             // at
#include <boost/mpl/int.hpp>           // int_
#include "Angle.hpp"
#include "Dimensions.hpp"
#include "Transform.hpp"
#include "Coordinates.hpp"
#include "Grid.hpp"
#include "Traits.hpp"

namespace dctl {
namespace board {

template<typename Board, typename /* ArgsTuple */, int SQ>
struct is_square
{
public:
        BOOST_STATIC_CONSTANT(auto, value = (SQ >= 0) && (SQ < Board::ExternalGrid::size));
};

template<typename Board, typename ArgsTuple, int SQ>
struct is_initial
{
private:
        typedef boost::mpl::integral_c<bool, 0> get_color;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, color = (boost::mpl::at<ArgsTuple, get_color>::type::value));
        BOOST_STATIC_CONSTANT(auto, row_min = 
                color? (Board::height - 1) - ((Board::height - Board::dmz) / 2 - 1) : 0
        );        
        BOOST_STATIC_CONSTANT(auto, row_max = 
                color? (Board::height - 1) : (Board::height - Board::dmz) / 2 - 1
        );
        BOOST_STATIC_CONSTANT(auto, row = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row)
        );

public:
        BOOST_STATIC_CONSTANT(auto, value = (row >= row_min) && (row <= row_max));
};

template<typename Board, typename ArgsTuple, int SQ>
struct is_row_mask
{
        typedef boost::mpl::integral_c<bool, 0> get_color;
        typedef boost::mpl::integral_c<int, 1> get_row;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, color = (boost::mpl::at<ArgsTuple, get_color>::type::value));
        BOOST_STATIC_CONSTANT(auto, row = (boost::mpl::at<ArgsTuple, get_row>::type::value));
        BOOST_STATIC_CONSTANT(auto, value = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row == 
                (color? (Board::height - 1) - row : row))
        );
};

template<typename Board, typename ArgsTuple, int SQ>
struct is_col_mask
{
        typedef boost::mpl::integral_c<bool, 0> get_color;
        typedef boost::mpl::integral_c<int, 1> get_column;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, color = (boost::mpl::at<ArgsTuple, get_color>::type::value));
        BOOST_STATIC_CONSTANT(auto, col = (boost::mpl::at<ArgsTuple, get_column>::type::value));
        BOOST_STATIC_CONSTANT(auto, value = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::col == 
                (color? (Board::width - 1) - col : col))
        );
};

template<typename Board, typename ArgsTuple, int SQ>
struct is_jump_group
{
private:
        typedef boost::mpl::integral_c<int, 0> get_from_sq;
        typedef typename Board::ExternalGrid Grid;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, from_sq = (boost::mpl::at<ArgsTuple, get_from_sq>::type::value));
        BOOST_STATIC_CONSTANT(auto, dest_sq = SQ);

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

template<typename Board, typename ArgsTuple, int SQ>
struct is_jump_start
{
private:
        typedef typename Board::ExternalGrid Grid;        
        typedef typename boost::mpl::at<ArgsTuple, boost::mpl::int_<0> >::type index;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting        
        BOOST_STATIC_CONSTANT(auto, offset = is_diagonal<index>::value? 2 : 4);
        
        BOOST_STATIC_CONSTANT(auto, row_min = is_up<index>::value? offset : 0);
        BOOST_STATIC_CONSTANT(auto, row_max = (Board::height - 1) - (is_down<index>::value? offset : 0));
        BOOST_STATIC_CONSTANT(auto, col_min = is_left<index>::value? offset : 0);
        BOOST_STATIC_CONSTANT(auto, col_max = (Board::width - 1) - (is_right<index>::value? offset : 0));

        BOOST_STATIC_CONSTANT(auto, row = (Square2Coordinates< Square<Grid, SQ> >::type::row));
        BOOST_STATIC_CONSTANT(auto, col = (Square2Coordinates< Square<Grid, SQ> >::type::col));

public:
        // a jump in direction <I> is possible if square <SQ> is within offset of the edges being approached
        BOOST_STATIC_CONSTANT(auto, value =
	        (row >= row_min) && (row <= row_max) &&
		(col >= col_min) && (col <= col_max)
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
