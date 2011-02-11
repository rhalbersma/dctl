#include "DeBruijn.h"

// The remainder of this file has been copied from the output of the following main program:

/*
#include "DeBruijn.h"

int main(void)
{
        DeBruijn<1>::generate_table();
        DeBruijn<2>::generate_table();
        DeBruijn<3>::generate_table();
        DeBruijn<4>::generate_table();
        DeBruijn<5>::generate_table();
        DeBruijn<6>::generate_table();
}
*/

const DeBruijn<1>::T DeBruijn<1>::SEQUENCE = 0x1;

const size_t DeBruijn<1>::TABLE[] = {
         0,  1
};

const DeBruijn<2>::T DeBruijn<2>::SEQUENCE = 0x3;

const size_t DeBruijn<2>::TABLE[] = {
         0,  1,  3,  2
};

const DeBruijn<3>::T DeBruijn<3>::SEQUENCE = 0x1d;

const size_t DeBruijn<3>::TABLE[] = {
         0,  1,  6,  2,  7,  5,  4,  3
};

const DeBruijn<4>::T DeBruijn<4>::SEQUENCE = 0x0f65;

const size_t DeBruijn<4>::TABLE[] = {
         0,  1, 11,  2, 14, 12,  8,  3,
        15, 10, 13,  7,  9,  6,  5,  4
};

const DeBruijn<5>::T DeBruijn<5>::SEQUENCE = 0x07dcd629;

const size_t DeBruijn<5>::TABLE[] = {
         0,  1, 23,  2, 29, 24, 14,  3,
        30, 27, 25, 18, 20, 15, 10,  4,
        31, 22, 28, 13, 26, 17, 19,  9,
        21, 12, 16,  8, 11,  7,  6,  5
};

const DeBruijn<6>::T DeBruijn<6>::SEQUENCE = 0x03f79d71b4cb0a89;

const size_t DeBruijn<6>::TABLE[] = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
};
