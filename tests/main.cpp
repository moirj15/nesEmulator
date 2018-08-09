#include <gtest/gtest.h>
#include "../cpu.h"

static u8 memory[0x1000];

/**
 * The accumulator should be zero on startup, since it was declared static.
 */
TEST(TestGetAccumulator, init_test)
{
    EXPECT_EQ(0, Cpu::get_accumulator());
}


int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
