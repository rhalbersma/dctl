template<typename T, size_t B>
class BIT_IS_GHOST
{
private: 
        enum {
                R = B % T::GHOST_MODULO,        // B = GHOST_MODULO * Q + R 
                BEGIN_LE = R < T::GHOST_LE,     // before leftmost bit of even rows
                BEGIN_LO = R < T::GHOST_LO,     // before leftmost bit of odd rows
                END_RE = R > T::GHOST_RE,       // beyond rightmost bit of even rows
                END_RO = R > T::GHOST_RO,       // beyond rightmost bit of odd rows
                END_BR = !(B < T::BIT_RANGE)    // beyond end of bit range
        };

public:
        static const bool VALUE = (END_RE && BEGIN_LO) || (END_RO && BEGIN_LE) || END_BR;
};

template<typename T, size_t B>
class BIT2COORD
{
private:
        enum {
                // B = GHOST_MODULO * Q + R0
                Q = B / T::GHOST_MODULO,       // number of row pairs                     
                R0 = B % T::GHOST_MODULO,      // bits from the left edge of the zeroth row
                R1 = R0 - T::GHOST_LO,         // bits from the left edge of the first row
                P = R0 >= T::GHOST_LO,         // determine whether bit R0 is in the zeroth or first row
                R = P? R1 : R0                 // bits from the left edge
        };

public:
        // twice the number of row pairs plus the row parity
        static const size_t ROW = 2 * Q + P;

        // twice the number of bits from the left edge plus the row parity XOR the opposite board coloring
        static const size_t COL = 2 * R + (P ^ !T::COLORING);
};

template<typename T, size_t ROW, size_t COL>
class COORD2BIT
{
private:
        //enum {
                // B = GHOST_MODULO * Q + R
                static const size_t Q = ROW / 2;                            // number of row pairs
                static const size_t P = ROW % 2;                            // row parity
                static const size_t S = COL / 2;
                static const size_t R0 = T::GHOST_LE + S;
                static const size_t R1 = T::GHOST_LO + S;
                static const size_t R = (P? R1 : R0) % T::GHOST_MODULO;
        //};

public:
        enum { VALUE = T::GHOST_MODULO * Q + R };
};
