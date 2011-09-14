#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Coordinates.hpp"
#include "Grid.hpp"
#include "Traits.hpp"

namespace dctl {
namespace board {

template<typename Board, typename /* Tuple */, int SQ>
class is_square
{
public:
        BOOST_STATIC_CONSTANT(auto, value = (SQ >= 0) && (SQ < Board::ExternalGrid::size));
};

template<typename Board, typename Tuple, int SQ>
class is_initial
{
private:
        BOOST_STATIC_CONSTANT(auto, C = Tuple::first);

        BOOST_STATIC_CONSTANT(auto, row_min = 
                C? (Board::height - 1) - ((Board::height - Board::dmz) / 2 - 1) : 0
        );        
        BOOST_STATIC_CONSTANT(auto, row_max = 
                C? (Board::height - 1) : (Board::height - Board::dmz) / 2 - 1
        );

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, row = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row)
        );

public:
        BOOST_STATIC_CONSTANT(auto, value = (row >= row_min) && (row <= row_max));
};

template<typename Board, typename Tuple, int SQ>
struct is_row_mask
{
        BOOST_STATIC_CONSTANT(auto, C = Tuple::first);
        BOOST_STATIC_CONSTANT(auto, row = Tuple::second);

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::row == 
                (C? (Board::height - 1) - row : row))
        );
};

template<typename Board, typename Tuple, int SQ>
struct is_col_mask
{
        BOOST_STATIC_CONSTANT(auto, C = Tuple::first);
        BOOST_STATIC_CONSTANT(auto, col = Tuple::second);

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Square2Coordinates< Square<typename Board::ExternalGrid, SQ> >::type::col == 
                (C? (Board::width - 1) - col : col))
        );
};

template<typename Board, typename Tuple, int SQ>
class is_jump_group
{
private:
        BOOST_STATIC_CONSTANT(auto, FROM = Tuple::first);
        BOOST_STATIC_CONSTANT(auto, DEST = SQ);

        typedef typename Board::ExternalGrid Grid;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, from_row = (Square2Coordinates< Square<Grid, FROM> >::type::row));
        BOOST_STATIC_CONSTANT(auto, dest_row = (Square2Coordinates< Square<Grid, DEST> >::type::row));
        BOOST_STATIC_CONSTANT(auto, from_col = (Square2Coordinates< Square<Grid, FROM> >::type::col));
        BOOST_STATIC_CONSTANT(auto, dest_col = (Square2Coordinates< Square<Grid, DEST> >::type::col));

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

template<typename Board, typename Tuple, int SQ>
class is_jump_start
{
private:
        BOOST_STATIC_CONSTANT(auto, I = Tuple::first);

        typedef typename Board::ExternalGrid Grid;

        BOOST_STATIC_CONSTANT(auto, offset = is_diagonal<I>::value? 2 : 4);
        BOOST_STATIC_CONSTANT(auto, row_min = is_up<I>::value? offset : 0);
        BOOST_STATIC_CONSTANT(auto, row_max = (Board::height - 1) - (is_down<I>::value? offset : 0));
        BOOST_STATIC_CONSTANT(auto, col_min = is_left<I>::value? offset : 0);
        BOOST_STATIC_CONSTANT(auto, col_max = (Board::width - 1) - (is_right<I>::value? offset : 0));
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
class square_to_bit
{
private:
        typedef typename Board::ExternalGrid E;
        typedef typename Board::InternalGrid I;

        // square coordinates within the external grid
        typedef typename Square2Coordinates< Square<E, SQ> >::type External;

        // rotated coordinates within the external grid
        typedef typename rotate<External, Board::angle>::type rotated;

public:
        // bit coordintaes re-interpreted within the internal grid
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Coordinates2Square< Coordinates<I, rotated::row, rotated::col> >::type::square)
        );
};

template<typename Board, int B>
class bit_to_square
{
private:
        typedef typename Board::InternalGrid I;
        typedef typename Board::ExternalGrid E;

        // bit coordinates within the internal grid
        typedef typename Square2Coordinates< Square<I, B> >::type Internal;

        // rotated coordinates within the external grid
        typedef typename rotate<Internal, Board::inverse_angle>::type rotated;

public:
        // square coordinates re-interpreted within the internal grid
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Coordinates2Square< Coordinates<E, rotated::row, rotated::col> >::type::square)
        );
};

}       // namespace board
}       // namespace dctl
