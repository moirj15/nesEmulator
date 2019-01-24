#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cpu.h"
#include "debugger.h"

#define EXPECT_EQ(a, b) assert(a == b)
#define EXPECT_NEQ(a, b) assert(a != b)

void addressing_tests() {
    Cpu::test_immediate();
    Cpu::test_zero_page();
    Cpu::test_zero_page_X();
    Cpu::test_zero_page_Y();
    Cpu::test_absolute();
    Cpu::test_absolute_X();
    Cpu::test_absolute_Y();
    Cpu::test_indexed_indirect();
    Cpu::test_indirect_indexed();
}

void and_test() {
    Cpu::test_init();

    u8 mem[] = {0x29, 0x00};

    Cpu::step(mem);

    EXPECT_EQ(Cpu::get_reg_a(), 0);
    EXPECT_EQ(Cpu::get_reg_status(), Cpu::ZERO_FLAG);

    u8 test2[] = {0x69, 0x80, 0x29, 0x80};

    Cpu::test_init();
    Cpu::step(test2);
    Cpu::step(test2);

    EXPECT_EQ(Cpu::get_reg_a(), 0x80);
    EXPECT_EQ(Cpu::get_reg_status(), Cpu::NEGATIVE_FLAG | Cpu::OVERFLOW_FLAG);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

    and_test();
    

    //printf("%s\n", dissassemble_inst(0xa2).c_str());

    //Cpu::testCpu(code);
	return EXIT_SUCCESS;
}
