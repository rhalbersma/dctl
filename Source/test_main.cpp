#include "gtest/gtest.h"

#define ACCEPTANCE_TEST 0

int main(int argc, char **argv) 
{
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
