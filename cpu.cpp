#include <string.h>
#include <assert.h>

#include "cpu.h"
#include "instructions.h"

namespace Cpu {

//static Cpu6502 cpu;
static const s32 cycles_per_second = 1790000;
static s32 cycle_count = 0;

////////////////////////////////////////////////////////////////////////////////
// UTILITIES
////////////////////////////////////////////////////////////////////////////////

/**
 * Get the correct value from memory depending on the opcode addressing mode.
 */
static u8 &get_value(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc++;
    switch (op.address_mode) {
    case IMMEDIATE:
        return mem[cpu->pc];
    case ZERO_PAGE:
        return mem[mem[cpu->pc]];
    case ZERO_PAGE_X:
        return mem[mem[cpu->pc] + cpu->x];
    case ZERO_PAGE_Y:
        return mem[mem[cpu->pc] + cpu->y];
    case ABSOLUTE: {
        u16 address = mem[cpu->pc];
        cpu->pc++;
        address |= mem[cpu->pc] << 8;
        return mem[address];
    }
    case ABSOLUTE_X: {
        u16 address = mem[cpu->pc];
        cpu->pc++;
        address |= mem[cpu->pc] << 8;
        return mem[address + cpu->x];
    }
    case ABSOLUTE_Y: {
        u16 address = mem[cpu->pc];
        cpu->pc++;
        address |= mem[cpu->pc] << 8;
        return mem[address + cpu->y];
    }
    case INDEXED_INDIRECT: { // (Indirect, X)
        u8 low_byte = mem[cpu->pc] + cpu->x;
        u8 high_byte = low_byte + 1;
        u16 address = mem[low_byte];
        address |= mem[high_byte] << 8;
        return mem[address];
    }
    case INDIRECT_INDEXED: { // (Indirect), Y
        u8 zp_address = mem[cpu->pc];
        u16 address = mem[zp_address];
        zp_address++;
        address |= mem[zp_address] << 8;
        return mem[address + cpu->y];
    }
    case ACCUMULATOR:
        return *((u8*)&cpu->a);
    }
}

static inline void set_if_zero(Cpu6502 *cpu, u8 val) {
    if (!val) {
        cpu->status |= ZERO_FLAG;
    }
}

static void new_page_check(Cpu6502 *cpu, s8 offset) {
    u16 old_pc = cpu->pc;
    cpu->pc += offset;
    s16 page_check = (s16)(old_pc & 0x00FF) + offset;
    if ((page_check > 0xFF) || (page_check < 0x00)) {
        cycle_count += 2;
    }

}

static void cond_branch(Cpu6502 *cpu, bool cond, u8 *mem) {
    cpu->pc++;
    if (cond) {
        s8 offset = mem[cpu->pc];
        new_page_check(cpu, offset);
        cycle_count += 2;
    }
    cycle_count++;
    cpu->pc++;
}

static void compare(Cpu6502 *cpu, u8 reg, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    u8 compare = reg - val;
    if (reg >= val) {
        cpu->status |= CARRY_FLAG; 
    }
    if (compare == 0) {
        cpu->status |= ZERO_FLAG;
    }
    cpu->status |= compare & NEGATIVE_FLAG;
    cpu->pc++;
}

static inline u8 pop_stack(Cpu6502 *cpu, u8 *mem) {
    cpu->sp++;
    return mem[0x0100 + cpu->sp];
}

static inline void push_stack(Cpu6502 *cpu, u8 *mem, u8 val) {
    mem[0x0100 + cpu->sp] = val;
    cpu->sp--;
}
////////////////////////////////////////////////////////////////////////////////
// INSTRUCTION IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * Add with carry instruction.
 */
static void adc(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 overflow_check = cpu->a & 0x80;
    cpu->a += get_value(cpu, mem, op) + (cpu->status & CARRY_FLAG);
    if (0x0F00 & cpu->a) {
        cpu->status |= CARRY_FLAG;
        cpu->a &= 0x00FF;
    }
    if (cpu->a == 0) {
        cpu->status |= ZERO_FLAG;
    }
    if (overflow_check != (cpu->a & 0x80)) {
        cpu->status |= OVERFLOW_FLAG;
    }
    if (overflow_check) {
        cpu->status |= NEGATIVE_FLAG;
    }
    cpu->pc++;
}

static void and_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->a &= get_value(cpu, mem, op);

    cpu->status |= cpu->a & NEGATIVE_FLAG;

    set_if_zero(cpu, cpu->a);

    cpu->pc++;
}

static void asl_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    bool old_7thbit = val >> 7;

    val <<= 1;

    if (old_7thbit) {
        cpu->status |= CARRY_FLAG;
    }
    else {
        cpu->status &= ~((u8)1);
    }
    if (!val) {
        cpu->status |= ZERO_FLAG;
    }
    cpu->status |= (val & NEGATIVE_FLAG);

    if (op.address_mode != ACCUMULATOR) {
        cpu->pc++;
    }
}


static void bcc_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, !(cpu->status & CARRY_FLAG), mem);
}

static void bcs_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, (cpu->status & CARRY_FLAG), mem);
}

static void beq_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, (cpu->status & ZERO_FLAG), mem);
}

static void bit_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    u8 tmp = cpu->a & val;
    if (tmp == 0) {
        cpu->status |= ZERO_FLAG;
    }
    else {
        cpu->status &= ~((u8)1);
    }
    cpu->status |= (tmp & OVERFLOW_FLAG) | (tmp & NEGATIVE_FLAG);
    cpu->pc++;
}

static void bmi_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, (cpu->status & NEGATIVE_FLAG), mem);
}

static void bne_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, !(cpu->status & ZERO_FLAG), mem);
}

static void bpl_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, !(cpu->status & NEGATIVE_FLAG), mem);
}

static void brk_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    push_stack(cpu, mem, cpu->pc);
    push_stack(cpu, mem, cpu->status);
    u16 irq_vector = mem[0xFFFE];
    irq_vector |= mem[0xFFFF] << 8;
    cpu->status |= BREAK_FLAG;
    cpu->pc++;
}

static void bvc_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, !(cpu->status & OVERFLOW_FLAG), mem);
}

static void bvs_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cond_branch(cpu, (cpu->status & OVERFLOW_FLAG), mem);
}

static void clc_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc++;
    (void)mem;
    (void)op;
    cpu->status &= ~((u8)CARRY_FLAG);
}

static void cld_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc++;
    (void)mem;
    (void)op;
    cpu->status &= ~((u8)DECIMAL_MODE_FLAG);
}

static void cli_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc++;
    (void)mem;
    (void)op;
    cpu->status &= ~((u8)INTERRUPT_DISSABLE_FLAG);
}

static void clv_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc++;
    (void)mem;
    (void)op;
    cpu->status &= ~((u8)OVERFLOW_FLAG);
}

static void cmp_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    compare(cpu, (u8)cpu->a, mem, op);
}

static void cpx_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    compare(cpu, (u8)cpu->x, mem, op);
}

static void cpy_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    compare(cpu, (u8)cpu->y, mem, op);
}

static void dec_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    val--;
    set_if_zero(cpu, val);
    cpu->status |= val & NEGATIVE_FLAG;
    cpu->pc++;
}

static void dex_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->x--;
    set_if_zero(cpu, cpu->x);
    cpu->status |= cpu->x & NEGATIVE_FLAG;
    cpu->pc++;
}

static void dey_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->y--;
    set_if_zero(cpu, cpu->y);
    cpu->status |= cpu->y & NEGATIVE_FLAG;
    cpu->pc++;
}

static void eor_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    cpu->a ^= val;
    set_if_zero(cpu, cpu->a);
    cpu->status |= cpu->a & NEGATIVE_FLAG;

    cpu->pc++;
}

static void inc_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    val++;
    set_if_zero(cpu, val);
    cpu->status |= val & NEGATIVE_FLAG;
    cpu->pc++;
}

static void inx_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->x++;
    set_if_zero(cpu, cpu->x);
    cpu->status |= cpu->x & NEGATIVE_FLAG;
    cpu->pc++;
}

static void iny_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->y++;
    set_if_zero(cpu, cpu->y);
    cpu->status |= cpu->y & NEGATIVE_FLAG;
    cpu->pc++;
}

static void jmp_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u16 address = 0;
    if (op.address_mode == ABSOLUTE) {
        u8 &val = get_value(cpu, mem, op);
        address = (&val) - mem;
    }
    else {
        cpu->pc++;
        u16 vector = mem[cpu->pc];
        cpu->pc++;
        vector |= mem[cpu->pc] << 8;
        // take care of case where lsb of target address lies on page boundry
        if ((0x00FF & vector) == 0x00FF) {
            address = mem[vector];
            address |= mem[vector & 0xFF00] << 8;
        }
        else {
            address = mem[vector];
            address |= mem[vector + 1] << 8;
        }
    }
    cpu->pc = address;
    cpu->pc++;
}

static void jsr_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u16 address = (&get_value(cpu, mem, op)) - mem;
    push_stack(cpu, mem, cpu->pc);
    cpu->pc = address;
}

static void lda_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    cpu->a = val;
    set_if_zero(cpu, cpu->a);
    cpu->status |= cpu->a & NEGATIVE_FLAG;
    cpu->pc++;
}

static void ldx_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    cpu->x = val;
    set_if_zero(cpu, cpu->x);
    cpu->status |= cpu->x & NEGATIVE_FLAG;
    cpu->pc++;
}

static void ldy_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    cpu->y = val;
    set_if_zero(cpu, cpu->y);
    cpu->status |= cpu->y & NEGATIVE_FLAG;
    cpu->pc++;
}

static void lsr_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    u8 tmp = val & CARRY_FLAG;
    if (tmp == 0) {
        cpu->status &= ~((u8)OVERFLOW_FLAG);
    }
    else {
        cpu->status |= CARRY_FLAG;
    }
    val >>= 1;
    set_if_zero(cpu, val);
    cpu->status |= val & NEGATIVE_FLAG;
    if (op.address_mode != ACCUMULATOR) {
        cpu->pc++;
    }
}

static void nop_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc++;
    (void)mem;
    (void)op;
}

static void ora_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    cpu->a |= val;
    set_if_zero(cpu, cpu->a);
    cpu->status |= val & NEGATIVE_FLAG;
    cpu->pc++;
}


static void pha_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    push_stack(cpu, mem, cpu->a);
    cpu->pc++;
}

static void php_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    push_stack(cpu, mem, cpu->status);
    cpu->pc++;
}

static void pla_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cpu->a = pop_stack(cpu, mem);
    set_if_zero(cpu, cpu->a);
    cpu->status |= cpu->a & NEGATIVE_FLAG;
    cpu->pc++;
}

static void plp_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->status = pop_stack(cpu, mem);
    cpu->pc++;
}

static void rol_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    u8 old_7th_bit = 0x80 & val;
    val <<= 1;
    val |= (cpu->status & CARRY_FLAG);
    if (old_7th_bit) {
        cpu->status |= CARRY_FLAG;
    }
    else {
        cpu->status &= ~((u8)CARRY_FLAG);
    }
    set_if_zero(cpu, val);
    cpu->status |= cpu->a & NEGATIVE_FLAG;
    if (op.address_mode != ACCUMULATOR) {
        cpu->pc++;
    }
}

static void ror_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    u8 old_0th_bit = 0x01 & val;
    val >>= 1;
    val |= (cpu->status & CARRY_FLAG) << 7;
    if (old_0th_bit) {
        cpu->status |= CARRY_FLAG;
    }
    else {
        cpu->status &= ~((u8)CARRY_FLAG);
    }
    set_if_zero(cpu, val);
    cpu->status |= cpu->a & NEGATIVE_FLAG;
    if (op.address_mode != ACCUMULATOR) {
        cpu->pc++;
    }
}

static void rti_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)op;
    cpu->status = pop_stack(cpu, mem);
    cpu->pc = pop_stack(cpu, mem);
    cpu->pc++;
}

static void rts_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->pc = pop_stack(cpu, mem);
    cpu->pc++;
}

static void sbc_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 overflow_check = cpu->a & 0x80;
    u8 &val = get_value(cpu, mem, op);
    cpu->a = cpu->a - val - (1 - (CARRY_FLAG & cpu->status));
    set_if_zero(cpu, cpu->a);
    if (0xFF00 & cpu->a) {
        cpu->status &= ~((u8)CARRY_FLAG);
    }
    if (overflow_check != (cpu->a & 0x80)) {
        cpu->status |= OVERFLOW_FLAG;
    }
    if (overflow_check) {
        cpu->status |= NEGATIVE_FLAG;
    }
    cpu->pc++;
}

static void sec_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)mem;
    (void)op;
    cpu->status |= CARRY_FLAG; 
    cpu->pc++;
}

static void sed_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)mem;
    (void)op;
    cpu->status |= DECIMAL_MODE_FLAG;
    cpu->pc++;
}

static void sei_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    (void)mem;
    (void)op;
    cpu->status |= INTERRUPT_DISSABLE_FLAG;
    cpu->pc++;
}

static void sta_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    val = (u8)cpu->a;
    cpu->pc++;
}

static void stx_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    val = (u8)cpu->x;
    cpu->pc++;
}

static void sty_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    u8 &val = get_value(cpu, mem, op);
    val = (u8)cpu->y;
    cpu->pc++;
}

static void tax_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->x = cpu->a;
    cpu->pc++;
}

static void tay_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->y = cpu->a;
    cpu->pc++;
}

static void tsx_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->x = cpu->pc;
    cpu->pc++;
}

static void txa_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->a = cpu->x;
    cpu->pc++;
}

static void txs_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->sp = cpu->x;
    cpu->pc++;
}

static void txy_op(Cpu6502 *cpu, u8 *mem, OpCode op) {
    cpu->a = cpu->y;
    cpu->pc++;
}

void init(Cpu6502 *cpu) {
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFF;
    cpu->status = 0;
    cpu->pc = 0xFFFA;
}

void test_init(Cpu6502 *cpu) {
    init(cpu);
    cpu->pc = 0;
}

void step(Cpu6502 *cpu, u8 *mem) {
    u8 op = mem[cpu->pc];
    switch (op) {

    // ADD
    case 0x69:case 0x65:case 0x75:case 0x6D:case 0x7D:case 0x79:case 0x61:
    case 0x71:
        adc(cpu, mem, opcodes[op]);
        break;

    // AND
    case 0x29:case 0x25:case 0x35:case 0x2D:case 0x3D:case 0x39:case 0x21:
    case 0x31:
        and_op(cpu, mem, opcodes[op]);
        break;

    // ASL
    case 0x0A:case 0x06:case 0x16:case 0x0E:case 0x1E:
        asl_op(cpu, mem, opcodes[op]);
        break;

    case 0x90:
        bcc_op(cpu, mem, opcodes[op]);
        break;

    case 0xB0:
        bcs_op(cpu, mem, opcodes[op]);
        break;

    case 0xF0:
        beq_op(cpu, mem, opcodes[op]);
        break;

    case 0x24:case 0x2C:
        bit_op(cpu, mem, opcodes[op]);
        break;

    case 0x30:
        bmi_op(cpu, mem, opcodes[op]);
        break;

    case 0xD0:
        bne_op(cpu, mem, opcodes[op]);
        break;

    case 0x10:
        bpl_op(cpu, mem, opcodes[op]);
        break;

    case 0x00:
        brk_op(cpu, mem, opcodes[op]);
        break;

    case 0x50:
        bvc_op(cpu, mem, opcodes[op]);
        break;

    case 0x70:
        bvs_op(cpu, mem, opcodes[op]);
        break;

    case 0x18:
        clc_op(cpu, mem, opcodes[op]);
        break;

    case 0xD8:
        cld_op(cpu, mem, opcodes[op]);
        break;

    case 0x58:
        cli_op(cpu, mem, opcodes[op]);
        break;

    case 0xB8:
        clv_op(cpu, mem, opcodes[op]);
        break;

    case 0xC9:case 0xC5:case 0xD5:case 0xCD:case 0xDD:case 0xD9:case 0xC1:
    case 0xD1:
        cmp_op(cpu, mem, opcodes[op]);
        break;

    case 0xE0:case 0xE4:case 0xEC:
        cpx_op(cpu, mem, opcodes[op]);
        break;

    case 0xC0:case 0xC4:case 0xCC:
        cpy_op(cpu, mem, opcodes[op]);
        break;

    case 0xC6:case 0xD6:case 0xCE:case 0xDE:
        dec_op(cpu, mem, opcodes[op]);
        break;

    case 0xCA:
        dex_op(cpu, mem, opcodes[op]);
        break;

    case 0x88:
        dey_op(cpu, mem, opcodes[op]);
        break;

    case 0x49:case 0x45:case 0x55:case 0x4D:case 0x5D:case 0x59:case 0x41:
    case 0x51:
        eor_op(cpu, mem, opcodes[op]);
        break;

    case 0xE6:case 0xF6:case 0xEE:case 0xFE:
        inc_op(cpu, mem, opcodes[op]);
        break;

    case 0xE8:
        inx_op(cpu, mem, opcodes[op]);
        break;

    case 0xC8:
        iny_op(cpu, mem, opcodes[op]);
        break;

    case 0x4C:case 0x6C:
        jmp_op(cpu, mem, opcodes[op]);
        break;

    case 0x20:
        jsr_op(cpu, mem, opcodes[op]);
        break;

    case 0xA9:case 0xA5:case 0xB5:case 0xAD:case 0xBD:case 0xB9:case 0xA1:
    case 0xB1:
        lda_op(cpu, mem, opcodes[op]);
        break;

    case 0xA2:case 0xA6:case 0xB6:case 0xAE:case 0xBE:
        ldx_op(cpu, mem, opcodes[op]);
        break;

    case 0xA0:case 0xA4:case 0xB4:case 0xAC:case 0xBC:
        ldy_op(cpu, mem, opcodes[op]);
        break;

    case 0x4A:case 0x46:case 0x56:case 0x4E:case 0x5E:
        lsr_op(cpu, mem, opcodes[op]);
        break;

    case 0xEA:
        nop_op(cpu, mem, opcodes[op]);
        break;

    case 0x09:case 0x05:case 0x15:case 0x0D:case 0x1D:case 0x19:case 0x01:
    case 0x11:
        ora_op(cpu, mem, opcodes[op]);
        break;

    case 0x48:
        pha_op(cpu, mem, opcodes[op]);
        break;

    case 0x08:
        php_op(cpu, mem, opcodes[op]);
        break;

    case 0x68:
        pla_op(cpu, mem, opcodes[op]);
        break;

    case 0x28:
        plp_op(cpu, mem, opcodes[op]);
        break;

    case 0x2A:case 0x26:case 0x36:case 0x2E:case 0x3E:
        rol_op(cpu, mem, opcodes[op]);
        break;

    case 0x6A:case 0x66:case 0x76:case 0x6E:case 0x7E:
        ror_op(cpu, mem, opcodes[op]);
        break;

    case 0x40:
        rti_op(cpu, mem, opcodes[op]);
        break;

    case 0x60:
        rts_op(cpu, mem, opcodes[op]);
        break;

    case 0xE9:case 0xE5:case 0xF5:case 0xED:case 0xFD:case 0xF9:case 0xE1:
    case 0xF1:
        sbc_op(cpu, mem, opcodes[op]);
        break;

    case 0x38:
        sec_op(cpu, mem, opcodes[op]);
        break;

    case 0xF8:
        sed_op(cpu, mem, opcodes[op]);
        break;

    case 0x78:
        sei_op(cpu, mem, opcodes[op]);
        break;

    case 0x85:case 0x95:case 0x8D:case 0x9D:case 0x99:case 0x81:case 0x91:
        sta_op(cpu, mem, opcodes[op]);
        break;

    case 0x86:case 0x96:case 0x8E:
        stx_op(cpu, mem, opcodes[op]);
        break;

    case 0x84:case 0x94:case 0x8C:
        sty_op(cpu, mem, opcodes[op]);
        break;

    case 0xAA:
        tax_op(cpu, mem, opcodes[op]);
        break;

    case 0xA8:
        tay_op(cpu, mem, opcodes[op]);
        break;

    case 0xBA:
        tsx_op(cpu, mem, opcodes[op]);
        break;

    case 0x8A:
        txa_op(cpu, mem, opcodes[op]);
        break;

    case 0x9A:
        txs_op(cpu, mem, opcodes[op]);
        break;

    case 0x98:
        txy_op(cpu, mem, opcodes[op]);         
        break;
      
    default:
        printf("ILLEGAL OPCODE 0x%X\n", op);
        assert(0);
        break;
    }
}

void run_until(Cpu6502 *cpu, u8 *mem, u16 address) {
    while (cpu->pc != address) {
        step(cpu, mem);
    }
}

////////////////////////////////////////////////////////////////////////////////
// TESTS
////////////////////////////////////////////////////////////////////////////////

// TODO: move these into the test file

void test_immediate(Cpu6502 *cpu) {
    u8 mem[] = {0, 1, 2, 3};
    cpu->pc = 0;
    OpCode op = {"", 0, 0, IMMEDIATE, 0};
    assert(get_value(cpu, mem, op) == 1);

    init(cpu);
}

void test_zero_page(Cpu6502 *cpu) {
    u8 mem[] = {0, 3, 2, 3, 4, 5};
    cpu->pc = 0;
    OpCode op = {"", 0, 0, ZERO_PAGE, 0};
    assert(get_value(cpu, mem, op) == 3);

    init(cpu);
}

void test_zero_page_X(Cpu6502 *cpu) {
    u8 mem[] = {0, 1, 2, 3, 4, 5};
    cpu->pc = 0;
    cpu->x = 4;
    OpCode op = {"", 0, 0, ZERO_PAGE_X, 0};
    assert(get_value(cpu, mem, op) == 5);

    init(cpu);
}

void test_zero_page_Y(Cpu6502 *cpu) {
    u8 mem[] = {0, 1, 2, 3, 4, 5};
    cpu->pc = 0;
    cpu->y = 4;
    OpCode op = {"", 0, 0, ZERO_PAGE_Y, 0};
    assert(get_value(cpu, mem, op) == 5);

    init(cpu);
}

void test_absolute(Cpu6502 *cpu) {
    u8 mem[] = {0, 1, 0, 3, 4, 5};
    cpu->pc = 0;
    OpCode op = {"", 0, 0, ABSOLUTE, 0};
    assert(get_value(cpu, mem, op) == 1);

    init(cpu);
}

void test_absolute_X(Cpu6502 *cpu) {
    u8 mem[] = {0, 1, 0, 3, 4, 5};
    cpu->pc = 0;
    cpu->x = 1;
    OpCode op = {"", 0, 0, ABSOLUTE_X, 0};
    assert(get_value(cpu, mem, op) == 0);

    init(cpu);
}

void test_absolute_Y(Cpu6502 *cpu) {
    u8 mem[] = {0, 1, 0, 3, 4, 5};
    cpu->pc = 0;
    cpu->y = 1;
    OpCode op = {"", 0, 0, ABSOLUTE_Y, 0};
    assert(get_value(cpu, mem, op) == 0);

    init(cpu);
}

void test_indexed_indirect(Cpu6502 *cpu) {
    u8 mem[] = {0, 0, 0, 3, 0, 0};
    cpu->pc = 0;
    cpu->x = 3;
    OpCode op = {"", 0, 0, INDEXED_INDIRECT, 0};
    assert(get_value(cpu, mem, op) == 3);

    init(cpu);
}

void test_indirect_indexed(Cpu6502 *cpu) {
    u8 mem[] = {0, 0, 0, 3, 0, 0};
    cpu->pc = 0;
    cpu->y = 3;
    OpCode op = {"", 0, 0, INDIRECT_INDEXED, 0};
    assert(get_value(cpu, mem, op) == 3);

    init(cpu);
}

}
