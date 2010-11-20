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
                Q = SQ / T::SQUARE_MODULO,              // number of row pairs                     
                R0 = SQ % T::SQUARE_MODULO,             // squares from the left edge of the zeroth row
                R1 = R0 - T::SQUARE_LO,                 // squares from the left edge of the first row
                P = R0 >= T::SQUARE_LO,                 // determine whether square R0 is in the zeroth or first row
                R = P? R1 : R0                          // squares from the left edge
        };

public:
        // twice the number of row pairs plus the row parity
        static const int ROW = 2 * Q + P;
               
        // twice the number of squares from the left edge plus the row parity XOR the opposite board coloring
        static const int COL = 2 * R + (P ^ !T::SQUARE_PARITY);
};

template<typename T, int ROW, int COL>
class COORD2SQUARE
{
private:
        enum {
                L0 = static_cast<int>(T::SQUARE_LE),    // the left edge of the zeroth row
                L1 = static_cast<int>(T::SQUARE_LO),    // the left edge of the first row
                M = static_cast<int>(T::SQUARE_MODULO), // squares per row pair
                P = ROW % 2,                            // row parity
                Q = ROW / 2,                            // number of row pairs
                L = P? L1 : L0,                         // the left edge
                S = COL / 2,                            // number of column pairs
                R = (L + S) % M                         // squares from the left edge
        };

public:
        static const int VALUE = M * Q + R;
};
