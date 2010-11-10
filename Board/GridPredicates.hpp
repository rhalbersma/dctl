template<typename T, int SQ>
class SQUARE_IS_VALID
{
public:
        static const bool VALUE = SQ < T::SQUARE_RANGE;
};

template<typename T, int SQ>
class SQUARE2COORD
{
private:
        enum {
                // SQ = SQUARE_MODULO * Q + R0
                Q = SQ / T::SQUARE_MODULO,              // number of row pairs                     
                R0 = SQ % T::SQUARE_MODULO,             // squares from the left edge of the zeroth row
                R1 = R0 - T::SQUARE_LO,                 // squares from the left edge of the first row
                P = R0 >= T::SQUARE_LO,                 // determine whether square R0 is in the zeroth or first row
                R = P? R1 : R0                          // squares from the left edge
        };

public:
        enum {
                // twice the number of row pairs plus the row parity
                ROW = 2 * Q + P,

                // twice the number of squares from the left edge plus the row parity XOR the opposite board coloring
                COL = 2 * R + (P ^ !T::SQUARE_PARITY)
        };
};

template<typename T, int ROW, int COL>
class COORD2SQUARE
{
private:
        enum {
                // SQ = SQUARE_MODULO * Q + R
                Q = ROW / 2,                            // number of row pairs
                P = ROW % 2,                            // row parity
                S = COL / 2,
                R0 = T::SQUARE_LE + S,
                R1 = T::SQUARE_LO + S,
                R = P? R1 : R0
        };

public:
        enum { VALUE = T::SQUARE_MODULO * Q + R };
};
