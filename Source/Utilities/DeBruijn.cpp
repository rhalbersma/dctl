#include "gtest/gtest.h"
#include "../../../Library/Source/Utilities/DeBruijn.h"
#include "../../../Library/Source/Utilities/IntegerTypes.h"

// The fixture for testing class SearchEndgame.
template<typename T>
class DeBruijnSequence: public ::testing::Test {
protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        DeBruijnSequence() {
        // You can do set-up work for each test here.
        }

        virtual ~DeBruijnSequence() {
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
TYPED_TEST_CASE(DeBruijnSequence, UnsignedIntegerTypes);

TYPED_TEST(DeBruijnSequence, ShiftTable)
{
        for (auto i = 0; i < NUM_BITS; ++i) {
                TypeParam b = TypeParam(1) << i;
                b *= DeBruijn<TypeParam>::SEQUENCE;
                b >>= DeBruijn<TypeParam>::SHIFT;
                EXPECT_EQ(i, DeBruijn<TypeParam>::TABLE[b]);
        }
}
