#include "gtest/gtest.h"
#include "../../../Library/src/utils/Bit.h"
#include "../../../Library/src/utils/IntegerTypes.h"

namespace dctl {
namespace bit {

// The fixture for testing class SearchEndgame.
template<typename T>
class Bit: public ::testing::Test {
protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Bit() {
        // You can do set-up work for each test here.
        }

        virtual ~Bit() {
        // You can do clean-up work that doesn't throw exceptions here.
        };

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        }

        virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
        }

        enum { NUM_BITS = 8 * sizeof(T) };

        // Objects declared here can be used by all tests in the test case for SearchEndgame.                
};

typedef ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t> UnsignedIntegerTypes;
TYPED_TEST_CASE(Bit, UnsignedIntegerTypes);

TYPED_TEST(Bit, IsZero)
{
        TypeParam b(0);
        EXPECT_EQ(true,  is_zero(b));
        EXPECT_EQ(false, is_single(b));
        EXPECT_EQ(false, is_double(b));
        EXPECT_EQ(false, is_multiple(b));
}

TYPED_TEST(Bit, IsSingle)
{
        for (auto i = 0; i < NUM_BITS; ++i) {
                TypeParam b = TypeParam(1) << i;
                EXPECT_EQ(false, is_zero(b));
                EXPECT_EQ(true,  is_single(b));
                EXPECT_EQ(false, is_double(b));
                EXPECT_EQ(false, is_multiple(b));
        }
}

TYPED_TEST(Bit, IsDouble)
{
        for (auto i = 0; i < NUM_BITS; ++i) {
                for (auto j = 0; j < NUM_BITS; ++j) {                                
                        TypeParam b = (TypeParam(1) << i) ^ (TypeParam(1) << j);
                        if (i == j) {
                                EXPECT_EQ(true, is_zero(b));
                        } else {
                                EXPECT_EQ(false, is_zero(b));
                                EXPECT_EQ(false, is_single(b));
                                EXPECT_EQ(true,  is_double(b));
                                EXPECT_EQ(true,  is_multiple(b));
                        }
                }
        }
}

TYPED_TEST(Bit, IsMultiple)
{
        for (auto i = 0; i < NUM_BITS; ++i) {
                for (auto j = 0; j < NUM_BITS - i; ++j) {
                        TypeParam b = ((TypeParam(1) << i) - TypeParam(1)) << j;
                        switch (i) {
                        case 0:
                                EXPECT_EQ(true, is_zero(b));
                                break;
                        case 1:
                                EXPECT_EQ(true, is_single(b));
                                break;
                        case 2:
                                EXPECT_EQ(true, is_double(b));
                                EXPECT_EQ(true, is_multiple(b));
                                break;
                        default:                        
                                EXPECT_EQ(false, is_zero(b));
                                EXPECT_EQ(false, is_single(b));
                                EXPECT_EQ(false, is_double(b));
                                EXPECT_EQ(true,  is_multiple(b));
                        }
                }
        }
        EXPECT_EQ(true, is_multiple(TypeParam(~0)));
}

TYPED_TEST(Bit, IndexDeBruijn)
{
        for (auto i = 0; i < NUM_BITS; ++i) {
                TypeParam b = TypeParam(1) << i;               
                EXPECT_EQ(i, index_DeBruijn(b));
        }
}

TYPED_TEST(Bit, CountKernighan)
{
        EXPECT_EQ(0, count_Kernighan(TypeParam(0)));

        for (auto i = 0; i < NUM_BITS; ++i) {
                TypeParam b = TypeParam(1) << i;
                EXPECT_EQ(1, count_Kernighan(b));                
        }

        for (auto i = 0; i < NUM_BITS; ++i) {
                for (auto j = 0; j < NUM_BITS; ++j) {                                
                        TypeParam b = (TypeParam(1) << i) ^ (TypeParam(1) << j);
                        if (i == j) {
                                EXPECT_EQ(0, count_Kernighan(b));
                        } else {
                                EXPECT_EQ(2, count_Kernighan(b));
                        }
                }
        }

        for (auto i = 0; i < NUM_BITS; ++i) {
                TypeParam b = (TypeParam(1) << i) - 1;
                EXPECT_EQ(i, count_Kernighan(b));
        }

        EXPECT_EQ(NUM_BITS, count_Kernighan(TypeParam(~0)));
}

}       // namespace bit
}       // namespace dctl
