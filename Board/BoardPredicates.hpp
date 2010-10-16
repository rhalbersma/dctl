#include "Direction.h"

template<typename Board, size_t SQ>
struct IS_INVALID_SQUARE
{
        static const bool VALUE = SQ >= Board::NUM_SQUARES;
};

template<typename Board, size_t B>
class IS_GHOST_BIT
{
private: 
        enum {
                R = B % Board::GHOST_MODULO,    // B = GHOST_MODULO * Q + R 
                EQ_RE = R == Board::GHOST_RE,   // right of even rows
                EQ_LO = R == Board::GHOST_LO,   // left of odd rows
                EQ_RO = R == Board::GHOST_RO,   // right of odd rows
                EQ_LE = R == Board::GHOST_LE,   // left of even rows
                EQ_MB = B >= Board::MIN_BITS    // beyond minimal number of bits
        };

public:
        static const bool VALUE = EQ_RE || EQ_LO || EQ_RO || EQ_LE || EQ_MB;
};

template<typename Board, bool C, size_t B>
class IS_INITIAL
{
private:
        enum {
                LOWER_BOUND = C? (Board::HEIGHT - 1) - ((Board::HEIGHT - Board::NEUTRAL_ZONE) / 2 - 1) : 0,
                UPPER_BOUND = C? (Board::HEIGHT - 1) : (Board::HEIGHT - Board::NEUTRAL_ZONE) / 2 - 1
        };

public:
        static const bool VALUE =
        	(LOWER_BOUND <= ROW_NUMBER<Board, B>::VALUE) &&
                (UPPER_BOUND >= ROW_NUMBER<Board, B>::VALUE)
        ;
};

template<typename Board, bool Color, size_t R, size_t B>
struct IS_ROW_MASK
{
        static const bool VALUE = ROW_NUMBER<Board, B>::VALUE == (Color? Board::HEIGHT - (R + 1) : R);
};

template<typename Board, bool Color, size_t C, size_t B>
struct IS_COL_MASK
{
        static const bool VALUE = COL_NUMBER<Board, B>::VALUE == (Color? Board::WIDTH - (C + 1) : C);
};

template<typename Board, size_t F, size_t D>
class IS_MAN_JUMP_GROUP
{
private: 
        enum {
                R1 = (ROW_NUMBER<Board, F>::VALUE - ROW_NUMBER<Board, D>::VALUE) % 4,
                C1 = (COL_NUMBER<Board, F>::VALUE - COL_NUMBER<Board, D>::VALUE) % 4,
                R2 = (R1 + 2) % 4,
                C2 = (C1 + 2) % 4
        };

public:
        // a (diagonal OR orthogonal!) man capture between square <F> and square <D> is possible if 
        static const bool VALUE =
        	(!R1 && !C1) || // row AND column numbers difference == 0 mod 4 (even number of captures)
                (!R2 && !C2)    // row AND column numbers difference == 2 mod 4 (odd number of captures)
        ;
};

template<typename Board, size_t Index, size_t B>
class IS_JUMPABLE
{
private:
        enum {
                OFFSET = Direction<Index>::IS_DIAGONAL? 2 : 4,
                ROW_LOWER_BOUND = Direction<Index>::IS_U? OFFSET : 0,
                ROW_UPPER_BOUND = (Board::HEIGHT - 1) - (Direction<Index>::IS_D? OFFSET : 0),
                COL_LOWER_BOUND = Direction<Index>::IS_L? OFFSET : 0,
                COL_UPPER_BOUND = (Board::WIDTH - 1) - (Direction<Index>::IS_R? OFFSET : 0)
        };

public:
        // a jump in direction <D> is possible if bit <B> is within OFFSET of the edges being approached
        static const bool VALUE =
	        (ROW_LOWER_BOUND <= ROW_NUMBER<Board, B>::VALUE) &&
		(ROW_UPPER_BOUND >= ROW_NUMBER<Board, B>::VALUE) &&
		(COL_LOWER_BOUND <= COL_NUMBER<Board, B>::VALUE) &&
		(COL_UPPER_BOUND >= COL_NUMBER<Board, B>::VALUE)
	;
};

template<typename Board, size_t B>
class ROW_NUMBER
{
private:
        enum {
                // B = GHOST_MODULO * Q + R
                Q = B / Board::GHOST_MODULO,            // number of row pairs                     
                R = B % Board::GHOST_MODULO,            // bits from the left edge of the first row
                C = R > Board::GHOST_RE                 // determine whether bit R is in the first or second row
        };

public:
        // twice the number of row pairs plus the parity of the row within the first row pair
        static const size_t VALUE = 2 * Q + C;
};

template<typename Board, size_t B>
class COL_NUMBER
{
private:
        enum {
                // B = GHOST_MODULO * Q1 + R1
                Q1 = B / Board::GHOST_MODULO,           // number of row pairs
                R1 = B % Board::GHOST_MODULO,           // bits from the left edge of the first row
                R2 = R1 - (Board::GHOST_LO + 1),        // squares from the left edge of the second row
                C1 = R1 > Board::GHOST_RE,              // determine whether bit R1 is in the first or second row
                R3 = C1? R2 : R1                        // squares from the left edge
        };

public:
        // twice the number of squares from the left edge plus the exclusive-OR parity of the row and the board
        enum { VALUE = 2 * R3 + (C1 ^ Board::PARITY) };
};
