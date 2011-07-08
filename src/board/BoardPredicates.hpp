#include "Angle.h"
#include "Coordinates.h"
#include "Grid.h"

namespace board {

template<typename T, int SQ>
class is_square
{
public:
        static const bool value = (SQ >= 0) && (SQ < T::SIZE);
};

template<typename T, bool C, int SQ>
class is_initial
{
private:
        enum {
                ROW_MIN = C? (T::HEIGHT - 1) - ((T::HEIGHT - T::DMZ) / 2 - 1) : 0,
                ROW_MAX = C? (T::HEIGHT - 1) : (T::HEIGHT - T::DMZ) / 2 - 1,
                ROW = Square2Coordiates< Square<T, SQ> >::type::row
        };

public:
        static const bool value = (ROW >= ROW_MIN) && (ROW <= ROW_MAX);
};

template<typename T, bool C, int ROW, int SQ>
struct is_row_mask
{
        static const bool value = Square2Coordinates< Square<T, SQ> >::type::row == (C? (T::HEIGHT - 1) - ROW : ROW);
};

template<typename T, bool C, int COL, int SQ>
struct is_col_mask
{
        static const bool value = Square2Coordinates< Square<T, SQ> >::type::col == (C? (T::WIDTH - 1) - COL : COL);
};

template<typename T, int FROM, int DEST>
class is_man_jump_group
{
private: 
        enum {
                FROM_ROW = Square2Coordinates< Square<T, FROM> >::type::row,
                DEST_ROW = Square2Coordinates< Square<T, DEST> >::type::row,
                FROM_COL = Square2Coordinates< Square<T, FROM> >::type::col,
                DEST_COL = Square2Coordinates< Square<T, DEST> >::type::col,
                R1 = (FROM_ROW - DEST_ROW) % 4,
                C1 = (FROM_COL - DEST_COL) % 4,
                R2 = (R1 + 2) % 4,
                C2 = (C1 + 2) % 4
        };

public:
        // a diagonal or orthogonal man capture between square <FROM> and square <DEST> is possible if 
        static const bool value =
        	(!R1 && !C1) || // row AND column numbers difference == 0 mod 4 (even number of captures)
                (!R2 && !C2)    // row AND column numbers difference == 2 mod 4 (odd number of captures)
        ;
};

template<typename T, int I, int SQ>
class is_jumpable
{
private:
        enum {
                OFFSET = Traits<I>::IS_DIAGONAL? 2 : 4,
                ROW_MIN = Traits<I>::IS_UP? OFFSET : 0,
                ROW_MAX = (T::HEIGHT - 1) - (Traits<I>::IS_DOWN? OFFSET : 0),
                COL_MIN = Traits<I>::IS_LEFT? OFFSET : 0,
                COL_MAX = (T::WIDTH - 1) - (Traits<I>::IS_RIGHT? OFFSET : 0),
                ROW = Square2Coordinates< Square<T, SQ> >::type::row,
                COL = Square2Coordinates< Square<T, SQ> >::type::col
        };

public:
        // a jump in direction <I> is possible if square <SQ> is within OFFSET of the edges being approached
        static const bool value =
	        (ROW >= ROW_MIN) &&
		(ROW <= ROW_MAX) &&
		(COL >= COL_MIN) &&
		(COL <= COL_MAX)
	;
};

template<typename T, int SQ>
class square2bit
{
private:
        typedef typename T::ExternalGrid E;
        typedef typename T::InternalGrid I;

        // square coordinates within the external grid
        typedef typename Square2Coordinates< Square<E, SQ> >::type External;

        // rotated coordinates within the external grid
        typedef typename Rotate<External, T::ANGLE>::type Rotated;

public:
        // bit coordintaes re-interpreted within the internal grid
        enum { value = Coordinates2Square< Coordinates<I, Rotated::row, Rotated::col> >::type::square };
};

template<typename T, int B>
class bit2square
{
private:
        typedef typename T::InternalGrid I;
        typedef typename T::ExternalGrid E;

        // bit coordinates within the internal grid
        typedef typename Square2Coordinates< Square<I, B> >::type Internal;

        // rotated coordinates within the external grid
        typedef typename Rotate<Internal, T::A_PRIME>::type Rotated;

public:
        // square coordinates re-interpreted within the internal grid
        enum { value = Coordinates2Square< Coordinates<E, Rotated::row, Rotated::col> >::type::square };
};

}       // namespace board
