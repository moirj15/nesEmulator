#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include <vector>

namespace Cpu {

enum FLAGS
{
    CARRY_FLAG = 1,
    ZERO_FLAG = 2,
    INTERRUPT_DISSABLE_FLAG = 4,
    DECIMAL_MODE_FLAG = 8,
    BREAK_FLAG = 32,
    OVERFLOW_FLAG = 64,
    NEGATIVE_FLAG = 128,
};

struct Cpu6502 {
    u16 pc;
    s16 a;
    u8 x;
    u8 y;
    u8 sp;
    u8 status;
};

/**
 * Init the cpu to the correct starting values.
 */
void init(Cpu6502 *cpu);

void test_init(Cpu6502 *cpu);

/**
 * Step through a single instruction.
 */
void step(Cpu6502 *cpu, u8 *mem);

/**
 * Run until the given address is reached.
 */
void run_until(Cpu6502 *cpu, u8 *mem, u16 address);

void test_immediate(Cpu6502 *cpu);
void test_zero_page(Cpu6502 *cpu);
void test_zero_page_X(Cpu6502 *cpu);
void test_zero_page_Y(Cpu6502 *cpu);
void test_absolute(Cpu6502 *cpu);
void test_absolute_X(Cpu6502 *cpu);
void test_absolute_Y(Cpu6502 *cpu);
void test_indexed_indirect(Cpu6502 *cpu);
void test_indirect_indexed(Cpu6502 *cpu);


}	

#endif
