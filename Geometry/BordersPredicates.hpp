namespace Geometry {

template<typename T, int B>
class BORDER_IS_GHOST
{
private: 
        enum {
                R = B % T::BORDER_MODULO,               // B = BORDER_MODULO * Q + R 
                BEGIN_LE = R < T::BORDER_LE,            // before leftmost bit of even rows
                BEGIN_LO = R < T::BORDER_LO,            // before leftmost bit of odd rows
                END_RE = R > T::BORDER_RE,              // beyond rightmost bit of even rows
                END_RO = R > T::BORDER_RO,              // beyond rightmost bit of odd rows
                END_BR = !(B < T::BORDER_RANGE)         // beyond end of bit range
        };

public:
        static const bool VALUE = (END_RE && BEGIN_LO) || (END_RO && BEGIN_LE) || END_BR;
};

template<typename T, int B>
class BIT2COORD
{
private:
        enum {
                Q = B / T::BORDER_MODULO,               // number of row pairs                     
                R0 = B % T::BORDER_MODULO,              // bits from the left edge of the zeroth row
                R1 = R0 - T::BORDER_LO,                 // bits from the left edge of the first row
                P = R1 >= 0,                            // determine whether bit R0 is in the zeroth or first row
                R = P? R1 : R0                          // bits from the left edge
        };

public:
        // twice the number of row pairs plus the row parity
        static const int ROW = 2 * Q + P;
                                
        // twice the number of bits from the left edge plus the row parity XOR the opposite board coloring
        static const int COL = static_cast<int>(2 * R + (P ^ !T::BORDER_PARITY));
};

template<typename T, int ROW, int COL>
class COORD2BIT
{
private:
        enum {
                L0 = static_cast<int>(T::BORDER_LE),    // the left edge of the zeroth row
                L1 = static_cast<int>(T::BORDER_LO),    // the left edge of the first row
                M = static_cast<int>(T::BORDER_MODULO), // bits per row pair
                P = ROW % 2,                            // row parity
                Q = ROW / 2,                            // number of row pairs
                L = P? L1 : L0,                         // the left edge
                S = COL / 2,                            // number of column pairs
                R = (L + S) % M                         // bits from the left edge
        };

public:
        static const int VALUE = M * Q + R;
};

}       // namespace Geometry
