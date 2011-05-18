#include "../../../Library/Source/Utilities/Bit.h"
#include "../../../Library/Source/Utilities/IntegerTypes.h"
#include "gtest/gtest.h"

TEST(Bit, IsZero)
{
        BitBoard b = 0;
        EXPECT_EQ(true,  Bit::is_zero(b));
        EXPECT_EQ(false, Bit::is_single(b));
        EXPECT_EQ(false, Bit::is_double(b));
        EXPECT_EQ(false, Bit::is_multiple(b));
}

TEST(Bit, IsSingle)
{
        for (size_t i = 0; i < 64; ++i) {
                BitBoard b = BitBoard(1) << i;
                EXPECT_EQ(false, Bit::is_zero(b));
                EXPECT_EQ(true,  Bit::is_single(b));
                EXPECT_EQ(false, Bit::is_double(b));
                EXPECT_EQ(false, Bit::is_multiple(b));
        }
}

TEST(Bit, IsDouble)
{
        for (size_t i = 0; i < 64; ++i) {
                for (size_t j = 0; j < 64; ++j) {                                
                        BitBoard b = (BitBoard(1) << i) ^ (BitBoard(1) << j);
                        if (i == j) {
                                EXPECT_EQ(true, Bit::is_zero(b));
                        } else {
                                EXPECT_EQ(false, Bit::is_zero(b));
                                EXPECT_EQ(false, Bit::is_single(b));
                                EXPECT_EQ(true,  Bit::is_double(b));
                                EXPECT_EQ(true,  Bit::is_multiple(b));
                        }
                }
        }
}

TEST(Bit, IsMultiple)
{
        for (size_t i = 0; i < 64; ++i) {
                for (size_t j = 0; j < 64 - i; ++j) {
                        BitBoard b = ((BitBoard(1) << i) - 1) << j;
                        switch (i) {
                        case 0:
                                EXPECT_EQ(true, Bit::is_zero(b));
                                break;
                        case 1:
                                EXPECT_EQ(true, Bit::is_single(b));
                                break;
                        case 2:
                                EXPECT_EQ(true, Bit::is_double(b));
                                EXPECT_EQ(true, Bit::is_multiple(b));
                                break;
                        default:                        
                                EXPECT_EQ(false, Bit::is_zero(b));
                                EXPECT_EQ(false, Bit::is_single(b));
                                EXPECT_EQ(false, Bit::is_double(b));
                                EXPECT_EQ(true,  Bit::is_multiple(b));
                        }
                }
        }
        EXPECT_EQ(true, Bit::is_multiple(BitBoard(~0)));
}

TEST(Bit, Index)
{
        for (size_t i = 0; i < 64; ++i) {
                BitBoard b = BitBoard(1) << i;
                EXPECT_EQ(i, Bit::index(b));
        }
}

TEST(Bit, Count)
{
        EXPECT_EQ(0, Bit::count(0));

        for (size_t i = 0; i < 64; ++i) {
                BitBoard b = BitBoard(1) << i;
                EXPECT_EQ(1, Bit::count(b));                
        }

        for (size_t i = 0; i < 64; ++i) {
                for (size_t j = 0; j < 64; ++j) {                                
                        BitBoard b = (BitBoard(1) << i) ^ (BitBoard(1) << j);
                        if (i == j) {
                                EXPECT_EQ(0, Bit::count(b));
                        } else {
                                EXPECT_EQ(2, Bit::count(b));
                        }
                }
        }

        for (size_t i = 0; i < 64; ++i) {
                BitBoard b = (BitBoard(1) << i) - 1;
                EXPECT_EQ(i, Bit::count(b));
        }

        EXPECT_EQ(64, Bit::count(BitBoard(~0)));
}
