#include "Coordinates.h"
#include "Grid.h"
#include "Traits.h"

namespace dctl {
namespace board {

template<typename Board, int SQ>
class is_square
{
public:
        static const bool value = (SQ >= 0) && (SQ < Board::ExternalGrid::SIZE);
};

template<typename Board, bool C, int SQ>
class is_initial
{
private:
        enum {
                ROW_MIN = C? (Board::HEIGHT - 1) - ((Board::HEIGHT - Board::DMZ) / 2 - 1) : 0,
                ROW_MAX = C? (Board::HEIGHT - 1) : (Board::HEIGHT - Board::DMZ) / 2 - 1,
                ROW = Square2Coordinates< Square<Board::ExternalGrid, SQ> >::type::row
        };

public:
        static const bool value = (ROW >= ROW_MIN) && (ROW <= ROW_MAX);
};

template<typename Board, bool C, int ROW, int SQ>
struct is_row_mask
{
        static const bool value = Square2Coordinates< Square<Board::ExternalGrid, SQ> >::type::row == (C? (Board::HEIGHT - 1) - ROW : ROW);
};

template<typename Board, bool C, int COL, int SQ>
struct is_col_mask
{
        static const bool value = Square2Coordinates< Square<Board::ExternalGrid, SQ> >::type::col == (C? (Board::WIDTH - 1) - COL : COL);
};

template<typename Board, int FROM, int DEST>
class is_man_jump_group
{
private:
        typedef typename Board::ExternalGrid Grid;
        enum {
                FROM_ROW = Square2Coordinates< Square<Grid, FROM> >::type::row,
                DEST_ROW = Square2Coordinates< Square<Grid, DEST> >::type::row,
                FROM_COL = Square2Coordinates< Square<Grid, FROM> >::type::col,
                DEST_COL = Square2Coordinates< Square<Grid, DEST> >::type::col,
                R1 = (FROM_ROW - DEST_ROW) % 4,
                C1 = (FROM_COL - DEST_COL) % 4,
                R2 = (R1 + 2) % 4,
                C2 = (C1 + 2) % 4
        };

public:
        // a diagonal or orthogonal man capture between square <FROM> and square <DEST> is possible if 
        static const bool value =
        	(!R1 && !C1) || // BOTH row AND column numbers difference == 0 mod 4 (even number of captures)
                (!R2 && !C2)    // BOTH row AND column numbers difference == 2 mod 4 (odd number of captures)
        ;
};

template<typename Board, int I, int SQ>
class is_jumpable
{
private:
        typedef typename Board::ExternalGrid Grid;
        enum {
                OFFSET = is_diagonal<I>::value? 2 : 4,
                ROW_MIN = is_up<I>::value? OFFSET : 0,
                ROW_MAX = (Board::HEIGHT - 1) - (is_down<I>::value? OFFSET : 0),
                COL_MIN = is_left<I>::value? OFFSET : 0,
                COL_MAX = (Board::WIDTH - 1) - (is_right<I>::value? OFFSET : 0),
                ROW = Square2Coordinates< Square<Grid, SQ> >::type::row,
                COL = Square2Coordinates< Square<Grid, SQ> >::type::col
        };

public:
        // a jump in direction <I> is possible if square <SQ> is within OFFSET of the edges being approached
        static const bool value =
	        (ROW >= ROW_MIN) && (ROW <= ROW_MAX) &&
		(COL >= COL_MIN) && (COL <= COL_MAX)
	;
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
        typedef typename Rotate<External, Board::ANGLE>::type Rotated;

public:
        // bit coordintaes re-interpreted within the internal grid
        enum { 
                value = Coordinates2Square< Coordinates<I, Rotated::row, Rotated::col> >::type::square 
        };
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
        typedef typename Rotate<Internal, Board::A_INV>::type Rotated;

public:
        // square coordinates re-interpreted within the internal grid
        enum { 
                value = Coordinates2Square< Coordinates<E, Rotated::row, Rotated::col> >::type::square 
        };
};

}       // namespace board
}       // namespace dctl
