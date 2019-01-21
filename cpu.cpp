#include <string.h>
#include <assert.h>

#include "cpu.h"
#include "instructions.h"

namespace Cpu {

static Cpu6502 cpu;
static const s32 cycles_per_second = 1790000;
static s32 cycle_count;

/**
 * Get the correct value from memory depending on the opcode addressing mode.
 */
static u8 *get_value(u8 *mem, OpCode op) {
    cpu.pc++;
    switch (op.address_mode) {
    case IMMEDIATE:
        return &mem[cpu.pc];
    case ZERO_PAGE:
        return &mem[mem[cpu.pc]];
    case ZERO_PAGE_X:
        return &mem[mem[cpu.pc] + cpu.x];
    case ZERO_PAGE_Y:
        return &mem[mem[cpu.pc] + cpu.y];
    case ABSOLUTE: {
        u16 address = mem[cpu.pc];
        cpu.pc++;
        address |= mem[cpu.pc] << 8;
        return &mem[address];
    }
    case ABSOLUTE_X: {
        u16 address = mem[cpu.pc];
        cpu.pc++;
        address |= mem[cpu.pc] << 8;
        return &mem[address + cpu.x];
    }
    case ABSOLUTE_Y: {
        u16 address = mem[cpu.pc];
        cpu.pc++;
        address |= mem[cpu.pc] << 8;
        return &mem[address + cpu.y];
    }
    case INDEXED_INDIRECT: { // (Indirect, X)
        u8 low_byte = mem[cpu.pc] + cpu.x;
        u8 high_byte = low_byte + 1;
        u16 address = mem[low_byte];
        address |= mem[high_byte] << 8;
        return &mem[address];
    }
    case INDIRECT_INDEXED: { // (Indirect), Y
        u8 zp_address = mem[cpu.pc];
        u16 address = mem[zp_address];
        zp_address++;
        address |= mem[zp_address] << 8;
        return &mem[address + cpu.y];
    }
    case ACCUMULATOR:
        return (u8*)&cpu.a;
    default:
        // TODO
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////
// INSTRUCTION IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////

static void nop() {
    cpu.pc++;
}

/**
 * Add with carry instruction.
 */
static void adc(u8 *mem, OpCode op) {
    u8 overflow_check = cpu.a & 0x80;
    cpu.a += *get_value(mem, op) + (cpu.status & CARRY_FLAG);
    if (0x0F00 & cpu.a) {
        cpu.status |= CARRY_FLAG;
        cpu.a &= 0x00FF;
    }
    if (cpu.a == 0) {
        cpu.status |= ZERO_FLAG;
    }
    if (overflow_check != (cpu.a & 0x80)) {
        cpu.status |= OVERFLOW_FLAG;
    }
    if (overflow_check) {
        cpu.status |= NEGATIVE_FLAG;
    }
    cpu.pc++;
}

void init(void) {
    cpu.a = 0;
    cpu.x = 0;
    cpu.y = 0;
    cpu.sp = 0xFF;
    cpu.status = 0;
    cpu.pc = 0xFFFA;
}

void step(u8 *mem) {
    u8 op = mem[cpu.pc];
    switch (op) {

    case 0xEA:
        nop();
        break;

    case 0x69:
    case 0x65:
    case 0x75:
    case 0x6D:
    case 0x7D:
    case 0x79:
    case 0x61:
    case 0x71:
        adc(mem, opcodes[op]);
        break;
    default:
        printf("ILLEGAL OPCODE 0x%X\n", op);
        break;
    }
}

void run_until(u8 *mem, u16 address) {
    while (cpu.pc != address) {
        step(mem);
    }
}

u8 get_reg_a() {
    return (u8)cpu.a;
}

u8 get_reg_x() {

}

u8 get_reg_y() {

}

u8 get_reg_sp() {

}

u8 get_reg_status() {

}

u8 get_reg_pc() {

}

////////////////////////////////////////////////////////////////////////////////
// TESTS
////////////////////////////////////////////////////////////////////////////////
void test_immediate() {
    u8 mem[] = {0, 1, 2, 3};
    cpu.pc = 0;
    OpCode op = {"", 0, 0, IMMEDIATE, 0};
    assert(*get_value(mem, op) == 1);

    init();
}

void test_zero_page() {
    u8 mem[] = {0, 3, 2, 3, 4, 5};
    cpu.pc = 0;
    OpCode op = {"", 0, 0, ZERO_PAGE, 0};
    assert(*get_value(mem, op) == 3);

    init();
}

void test_zero_page_X() {
    u8 mem[] = {0, 1, 2, 3, 4, 5};
    cpu.pc = 0;
    cpu.x = 4;
    OpCode op = {"", 0, 0, ZERO_PAGE_X, 0};
    assert(*get_value(mem, op) == 5);

    init();
}

void test_zero_page_Y() {
    u8 mem[] = {0, 1, 2, 3, 4, 5};
    cpu.pc = 0;
    cpu.y = 4;
    OpCode op = {"", 0, 0, ZERO_PAGE_Y, 0};
    assert(*get_value(mem, op) == 5);

    init();
}

void test_absolute() {
    u8 mem[] = {0, 1, 0, 3, 4, 5};
    cpu.pc = 0;
    OpCode op = {"", 0, 0, ABSOLUTE, 0};
    assert(*get_value(mem, op) == 1);

    init();
}

void test_absolute_X() {
    u8 mem[] = {0, 1, 0, 3, 4, 5};
    cpu.pc = 0;
    cpu.x = 1;
    OpCode op = {"", 0, 0, ABSOLUTE_X, 0};
    assert(*get_value(mem, op) == 0);

    init();
}

void test_absolute_Y() {
    u8 mem[] = {0, 1, 0, 3, 4, 5};
    cpu.pc = 0;
    cpu.y = 1;
    OpCode op = {"", 0, 0, ABSOLUTE_Y, 0};
    assert(*get_value(mem, op) == 0);

    init();
}

void test_indexed_indirect() {
    u8 mem[] = {0, 0, 0, 3, 0, 0};
    cpu.pc = 0;
    cpu.x = 3;
    OpCode op = {"", 0, 0, INDEXED_INDIRECT, 0};
    assert(*get_value(mem, op) == 3);

    init();
}

void test_indirect_indexed() {
    u8 mem[] = {0, 0, 0, 3, 0, 0};
    cpu.pc = 0;
    cpu.y = 3;
    OpCode op = {"", 0, 0, INDIRECT_INDEXED, 0};
    assert(*get_value(mem, op) == 3);

    init();
}

}
