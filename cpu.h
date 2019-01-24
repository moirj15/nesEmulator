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
void init(void);

void test_init();

/**
 * Step through a single instruction.
 */
void step(u8 *mem);

/**
 * Run until the given address is reached.
 */
void run_until(u8 *mem, u16 address);

/**
 * Getter for the accumulator.
 */
u8 get_reg_a();
u8 get_reg_x();
u8 get_reg_y();
u8 get_reg_sp();
u8 get_reg_status();
u16 get_reg_pc();

void test_immediate();
void test_zero_page();
void test_zero_page_X();
void test_zero_page_Y();
void test_absolute();
void test_absolute_X();
void test_absolute_Y();
void test_indexed_indirect();
void test_indirect_indexed();


}	

#endif
