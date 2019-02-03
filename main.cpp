#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cpu.h"
#include "debugger.h"

#define EXPECT_EQ(a, b) assert((a) == (b))
#define EXPECT_NEQ(a, b) assert((a) != (b))

void addressing_tests() {
    printf("TESTING ADDRESS MODES\n");
    Cpu::Cpu6502 cpu;
    Cpu::init(&cpu);
    Cpu::test_immediate(&cpu);
    Cpu::test_zero_page(&cpu);
    Cpu::test_zero_page_X(&cpu);
    Cpu::test_zero_page_Y(&cpu);
    Cpu::test_absolute(&cpu);
    Cpu::test_absolute_X(&cpu);
    Cpu::test_absolute_Y(&cpu);
    Cpu::test_indexed_indirect(&cpu);
    Cpu::test_indirect_indexed(&cpu);
}

void and_test() {
    printf("TESTING AND\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);

    u8 mem[] = {0x29, 0x00};

    Cpu::step(&cpu, mem);

    EXPECT_EQ(cpu.a, 0);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);

    u8 test2[] = {0x69, 0x80, 0x29, 0x80};

    Cpu::test_init(&cpu);
    Cpu::step(&cpu, test2);
    Cpu::step(&cpu, test2);

    EXPECT_EQ(cpu.a, 0x80);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG | Cpu::OVERFLOW_FLAG);
}

void asl_test() {
    printf("TESTING ASL\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.a = 0x80;
    u8 carry_zero_test[] = {0x0A};
    Cpu::step(&cpu, carry_zero_test);

    EXPECT_EQ(cpu.a, 0x00);
    EXPECT_EQ(cpu.status, Cpu::CARRY_FLAG | Cpu::ZERO_FLAG);
    
    Cpu::test_init(&cpu);
    u8 negative_test[] = {0x0A};
    cpu.a = 0x40;
    Cpu::step(&cpu, negative_test);
    EXPECT_EQ(cpu.a, 0x80);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

    and_test();
    asl_test();
    

    //printf("%s\n", dissassemble_inst(0xa2).c_str());

    //Cpu::testCpu(code);
	return EXIT_SUCCESS;
}
