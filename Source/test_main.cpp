#include "gtest/gtest.h"

#define INTEGRATION_TEST 0

int main(int argc, char **argv) 
{
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
