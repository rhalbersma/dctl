// The remainder of this file has been copied from the output of the following main program:

/*
#include "../../DCTL/src/bit/DeBruijn.h"

int main()
{
        using namespace dctl::bit; 
        DeBruijn<uint8_t >::generate_table();
        DeBruijn<uint16_t>::generate_table();
        DeBruijn<uint32_t>::generate_table();
        DeBruijn<uint64_t>::generate_table();
}
*/

#include "DeBruijn.h"

namespace dctl {
namespace bit {

template<>
const unsigned char DeBruijn<unsigned char>::SEQUENCE = 0x1d;

template<>
const int DeBruijn<unsigned char>::TABLE[] = {
         0,  1,  6,  2,  7,  5,  4,  3
};

template<>
const unsigned short DeBruijn<unsigned short>::SEQUENCE = 0x0f65;

template<>
const int DeBruijn<unsigned short>::TABLE[] = {
         0,  1, 11,  2, 14, 12,  8,  3,
        15, 10, 13,  7,  9,  6,  5,  4
};

template<>
const unsigned int DeBruijn<unsigned int>::SEQUENCE = 0x07dcd629;

template<>
const int DeBruijn<unsigned int>::TABLE[] = {
         0,  1, 23,  2, 29, 24, 14,  3,
        30, 27, 25, 18, 20, 15, 10,  4,
        31, 22, 28, 13, 26, 17, 19,  9,
        21, 12, 16,  8, 11,  7,  6,  5
};

template<>
const unsigned __int64 DeBruijn<unsigned __int64>::SEQUENCE = 0x03f79d71b4cb0a89
;

template<>
const int DeBruijn<unsigned __int64>::TABLE[] = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
};

}       // namespace bit
}       // namespace dctl