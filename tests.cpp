#include "tests.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cpu.h"
#include "debugger.h"

namespace Tests {

#define EXPECT_EQ(a, b) assert((a) == (b))
#define EXPECT_NEQ(a, b) assert((a) != (b))

static void addressing_tests() {
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

static void and_test() {
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

static void asl_test() {
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

static void bcc_test() {
    printf("TESTING BCC\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 branch_test[] = {
        0x90, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void bcs_test() {
    printf("TESTING BCS\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status |= Cpu::CARRY_FLAG;
    u8 branch_test[] = {
        0xB0, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void beq_test() {
    printf("TESTING BEQ\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status |= Cpu::ZERO_FLAG;
    u8 branch_test[] = {
        0xF0, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void bit_test() {
    printf("TESTING BIT\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);

    u8 bit_test_zero[] = {
        0x24, 0x02, 0x00
    };
    Cpu::step(&cpu, bit_test_zero);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);

    Cpu::test_init(&cpu);

    cpu.a = 0xFF;
    u8 bit_test_VN_flags[] = {
        0x24, 0x02, 0xFF
    };
    Cpu::step(&cpu, bit_test_VN_flags);
    EXPECT_EQ(cpu.status, Cpu::OVERFLOW_FLAG | Cpu::NEGATIVE_FLAG);
    
}

static void bmi_test() {
    printf("TESTING BMI\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status |= Cpu::NEGATIVE_FLAG;
    u8 branch_test[] = {
        0x30, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void bne_test() {
    printf("TESTING BNE\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 branch_test[] = {
        0xD0, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void bpl_test() {
    printf("TESTING BPL\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 branch_test[] = {
        0x10, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void bvc_test() {
    printf("TESTING BVC\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 branch_test[] = {
        0x50, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void bvs_test() {
    printf("TESTING BVS\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status |= Cpu::OVERFLOW_FLAG;
    u8 branch_test[] = {
        0x70, 0x01
    };
    Cpu::step(&cpu, branch_test);
    EXPECT_EQ(cpu.pc, 0x03);
}

static void clc_test() {
    printf("TESTING CLC\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status = Cpu::OVERFLOW_FLAG;
    cpu.status |= Cpu::CARRY_FLAG;
    u8 clc[] = {
        0x18,
    };
    Cpu::step(&cpu, clc);
    EXPECT_EQ(cpu.status, Cpu::OVERFLOW_FLAG);
}

static void cld_test() {
    printf("TESTING CLD\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status = Cpu::OVERFLOW_FLAG;
    cpu.status |= Cpu::DECIMAL_MODE_FLAG;
    u8 cld[] = {
        0xD8,
    };
    Cpu::step(&cpu, cld);
    EXPECT_EQ(cpu.status, Cpu::OVERFLOW_FLAG);
}

static void cli_test() {
    printf("TESTING CLI\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status = Cpu::OVERFLOW_FLAG;
    cpu.status |= Cpu::INTERRUPT_DISSABLE_FLAG;
    u8 cli[] = {
        0x58
    };
    Cpu::step(&cpu, cli);
    EXPECT_EQ(cpu.status, Cpu::OVERFLOW_FLAG);
}

static void clv_test() {
    printf("TESTING CLV\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.status = Cpu::NEGATIVE_FLAG;
    cpu.status |= Cpu::OVERFLOW_FLAG;
    u8 clv[] = {
        0xB8
    };
    Cpu::step(&cpu, clv);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
}

static void cmp_test() {
    printf("TESTING CMP\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.a = 0;
    u8 equality_test[] = {
        0xC9, 0x00
    };
    Cpu::step(&cpu, equality_test);
    EXPECT_EQ(cpu.status, Cpu::CARRY_FLAG | Cpu::ZERO_FLAG);

    Cpu::test_init(&cpu);
    cpu.a = 0x80;
    u8 neg_acc_test[] = {
        0xC9, 0x00
    };
    Cpu::step(&cpu, neg_acc_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG | Cpu::CARRY_FLAG);

    Cpu::test_init(&cpu);
    cpu.a = 0x00;
    u8 acc_less_than_test[] = {
        0xC9, 0x0a
    };
    Cpu::step(&cpu, acc_less_than_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
}

static void cpx_test() {
    printf("TESTING CPX\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.x = 0;
    u8 equality_test[] = {
        0xE0, 0x00
    };
    Cpu::step(&cpu, equality_test);
    EXPECT_EQ(cpu.status, Cpu::CARRY_FLAG | Cpu::ZERO_FLAG);

    Cpu::test_init(&cpu);
    cpu.x = 0x80;
    u8 neg_acc_test[] = {
        0xE0, 0x00
    };
    Cpu::step(&cpu, neg_acc_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG | Cpu::CARRY_FLAG);

    Cpu::test_init(&cpu);
    cpu.x = 0x00;
    u8 acc_less_than_test[] = {
        0xE0, 0x0a
    };
    Cpu::step(&cpu, acc_less_than_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
}

static void cpy_test() {
    printf("TESTING CPY\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.y = 0;
    u8 equality_test[] = {
        0xC0, 0x00
    };
    Cpu::step(&cpu, equality_test);
    EXPECT_EQ(cpu.status, Cpu::CARRY_FLAG | Cpu::ZERO_FLAG);

    Cpu::test_init(&cpu);
    cpu.y = 0x80;
    u8 neg_acc_test[] = {
        0xC0, 0x00
    };
    Cpu::step(&cpu, neg_acc_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG | Cpu::CARRY_FLAG);

    Cpu::test_init(&cpu);
    cpu.y = 0x00;
    u8 acc_less_than_test[] = {
        0xC0, 0x0a
    };
    Cpu::step(&cpu, acc_less_than_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
}

static void dec_test() {
    printf("TESTING DEC\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 dec_underflow_test[] = {
        0xC6, 0x02, 0x00
    };
    Cpu::step(&cpu, dec_underflow_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(dec_underflow_test[2], 0xFF);

    Cpu::test_init(&cpu);
    u8 dec_zero_test[] = {
        0xC6, 0x01
    };
    
    Cpu::step(&cpu, dec_zero_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(dec_zero_test[1], 0x00);
}

static void dex_test() {
    printf("TESTING DEX\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.x = 0x00;
    u8 dex_underflow_test[] = {
        0xCA
    };
    Cpu::step(&cpu, dex_underflow_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(cpu.x, 0xFF);

    Cpu::test_init(&cpu);
    
    u8 dex_zero_test[] = {
        0xCA
    };
    
    cpu.x = 0x01;
    Cpu::step(&cpu, dex_zero_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(cpu.x, 0x00);
}

static void dey_test() {
    printf("TESTING DEY\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.y = 0x00;
    u8 dey_underflow_test[] = {
        0x88
    };
    Cpu::step(&cpu, dey_underflow_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(cpu.y, 0xFF);

    Cpu::test_init(&cpu);
    
    u8 dey_zero_test[] = {
        0x88
    };
    
    cpu.y = 0x01;
    Cpu::step(&cpu, dey_zero_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(cpu.y, 0x00);
}

static void eor_test() {
    printf("TESTING EOR\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.a = 0xFF;
    u8 zero_flag_test[] = {
        0x49, 0xFF
    };
    Cpu::step(&cpu, zero_flag_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(cpu.a, 0);

    Cpu::test_init(&cpu);
    cpu.a = 0x7F;
    u8 neg_flag_test[] = {
        0x49, 0xFF
    };
    Cpu::step(&cpu, neg_flag_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(cpu.a, 0x80);
}

static void inc_test() {
    printf("TESTING INC\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 inc_zero_test[] = {
        0xE6, 0x02, 0xFF
    };
    Cpu::step(&cpu, inc_zero_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(inc_zero_test[2], 0x00);

    Cpu::test_init(&cpu);
    u8 inc_neg_test[] = {
        0xE6, 0x02, 0x7F
    };
    
    Cpu::step(&cpu, inc_neg_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(inc_neg_test[2], 0x80);
}

static void inx_test() {
    printf("TESTING INX\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.x = 0xFF;
    u8 inx_zero_test[] = {
        0xE8
    };
    Cpu::step(&cpu, inx_zero_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(cpu.x, 0x00);

    Cpu::test_init(&cpu);
    cpu.x = 0x7F;
    u8 inx_neg_test[] = {
        0xE8
    };
    
    Cpu::step(&cpu, inx_neg_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(cpu.x, 0x80);
}

static void iny_test() {
    printf("TESTING INY\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    cpu.y = 0xFF;
    u8 iny_zero_test[] = {
        0xC8
    };
    Cpu::step(&cpu, iny_zero_test);
    EXPECT_EQ(cpu.status, Cpu::ZERO_FLAG);
    EXPECT_EQ(cpu.y, 0x00);

    Cpu::test_init(&cpu);
    cpu.y = 0x7F;
    u8 iny_neg_test[] = {
        0xC8
    };
    
    Cpu::step(&cpu, iny_neg_test);
    EXPECT_EQ(cpu.status, Cpu::NEGATIVE_FLAG);
    EXPECT_EQ(cpu.y, 0x80);
}

static void jmp_test() {
    printf("TESTING JMP\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
    u8 absolute_test[] = {
        0x4C, 0x03, 0x00, 0xAB
    };
    Cpu::step(&cpu, absolute_test);
    EXPECT_EQ(cpu.pc, 0x04);

    Cpu::test_init(&cpu);
    u8 indirect_test[] = {
        0x6C, 0x03, 0x00, 0x05, 0x00
    };
    Cpu::step(&cpu, indirect_test);
    EXPECT_EQ(cpu.pc, 0x06);
}

static void lda_test() {
    printf("TESTING LDA\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
}

static void ldx_test() {
    printf("TESTING LDX\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
}

static void ldy_test() {
    printf("TESTING LDY\n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
}

void run() {
    addressing_tests();
    and_test();
    asl_test();
    bcc_test();
    bcs_test();
    beq_test();
    bit_test();
    bmi_test();
    bne_test();
    bpl_test();
    bvc_test();
    bvs_test();
    clc_test();
    cld_test();
    cli_test();
    clv_test();
    cmp_test();
    cpx_test();
    cpy_test();
    dec_test();
    dex_test();
    dey_test();
    eor_test();
    inc_test();
    inx_test();
    iny_test();
    jmp_test();
}

#if 0
static void _test() {
    printf("TESTING \n");
    Cpu::Cpu6502 cpu;
    Cpu::test_init(&cpu);
}

#endif


}
