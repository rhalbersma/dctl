template<typename T, int B>
class BIT_IS_GHOST
{
private: 
        enum {
                R = B % T::BIT_MODULO,                  // B = BIT_MODULO * Q + R 
                BEGIN_LE = R < T::BIT_LE,               // before leftmost bit of even rows
                BEGIN_LO = R < T::BIT_LO,               // before leftmost bit of odd rows
                END_RE = R > T::BIT_RE,                 // beyond rightmost bit of even rows
                END_RO = R > T::BIT_RO,                 // beyond rightmost bit of odd rows
                END_BR = !(B < T::BIT_RANGE)            // beyond end of bit range
        };

public:
        static const bool VALUE = (END_RE && BEGIN_LO) || (END_RO && BEGIN_LE) || END_BR;
};

template<typename T, int B>
class BIT2COORD
{
private:
        enum {
                Q = B / T::BIT_MODULO,                  // number of row pairs                     
                R0 = B % T::BIT_MODULO,                 // bits from the left edge of the zeroth row
                R1 = R0 - T::BIT_LO,                    // bits from the left edge of the first row
                P = R1 >= 0,                            // determine whether bit R0 is in the zeroth or first row
                R = P? R1 : R0                          // bits from the left edge
        };

public:
        enum {
                // twice the number of row pairs plus the row parity
                ROW = 2 * Q + P,

                // twice the number of bits from the left edge plus the row parity XOR the opposite board coloring
                COL = 2 * R + (P ^ !T::BIT_PARITY)
        };
};

template<typename T, int ROW, int COL>
class COORD2BIT
{
private:
        enum {
                Q = ROW / 2,                            // number of row pairs
                P = ROW % 2,                            // row parity
                S = COL / 2,                            // number of column pairs
                R0 = T::BIT_LE + S,                     // bits from the left edge of the zeroth row
                R1 = T::BIT_LO + S,                     // bits from the left edge of the first row
                R = (P? R1 : R0) % T::BIT_MODULO        // bits from the left edge
        };

public:
        enum { VALUE = T::BIT_MODULO * Q + R };
};
