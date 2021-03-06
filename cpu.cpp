#include <string.h>

#include "cpu.h"
#include "debugger.h"

namespace Cpu {

// Time saving macros
#define tick2() tick();tick();
#define tick3() tick();tick();tick();
#define tick4() tick();tick();tick();tick();
#define tick5() tick();tick();tick();tick();tick();
#define tick6() tick();tick();tick();tick();tick();tick();
#define tick7() tick();tick();tick();tick();tick();tick();tick();

//-----------------------------------------------------------------------------
// Load/Store Operations
//-----------------------------------------------------------------------------

// Load accumulator instructions
#define LDA_IMM		0xA9
#define LDA_ZP		0xA5
#define LDA_ZP_X	0xB5
#define LDA_ABS		0xAD
#define LDA_ABS_X	0xBD
#define LDA_ABS_Y	0xB9
#define LDA_IND_X	0xA1
#define LDA_IND_Y	0xB1

// Load X instructions
#define LDX_IMM		0xA2
#define LDX_ZP		0xA6
#define LDX_ZP_Y	0xB6
#define LDX_ABS		0xAE
#define LDX_ABS_Y	0xBE

// Load Y instructions
#define LDY_IMM		0xA0
#define LDY_ZP		0xA4
#define LDY_ZP_X	0xB4
#define LDY_ABS		0xAC
#define LDY_ABS_X	0xBC

// Store accumulator instructions
#define STA_ZP		0x85
#define STA_ZP_X	0x95
#define STA_ABS		0x8D
#define STA_ABS_X	0x9D
#define STA_ABS_Y	0x99
#define STA_IND_X	0x81
#define STA_IND_Y	0x91

// Store X instructions
#define STX_ZP		0x86
#define STX_ZP_Y	0x96
#define STX_ABS		0x8E

// Store Y instructions
#define STY_ZP		0x84
#define STY_ZP_X	0x94
#define STY_ABS		0x8C

//-----------------------------------------------------------------------------
// Register Transfer instructions
//-----------------------------------------------------------------------------
#define TAX			0xAA
#define TAY			0xA8
#define TXA			0x8A
#define TYA			0x98

//-----------------------------------------------------------------------------
// Stack operations
//-----------------------------------------------------------------------------
#define TSX			0xBA
#define TXS			0x9A
#define PHA			0x48
#define PHP			0x08
#define PLA			0x68
#define PLP			0x28

//-----------------------------------------------------------------------------
// Logical operations
//-----------------------------------------------------------------------------

// Logical AND instructions
#define AND_IMM		0x29
#define AND_ZP		0x25
#define AND_ZP_X	0x35
#define AND_ABS		0x2D
#define AND_ABS_X	0x3D
#define AND_ABS_Y	0x39
#define AND_IND_X	0x21
#define AND_IND_Y	0x31

// Logical exclusive or instructions
#define EOR_IMM		0x49
#define EOR_ZP		0x45
#define EOR_ZP_X	0x55
#define EOR_ABS		0x4D
#define EOR_ABS_X	0x5D
#define EOR_ABS_Y	0x59
#define EOR_IND_X	0x41
#define EOR_IND_Y	0x51

// Logical or instructions
#define ORA_IMM		0x09
#define ORA_ZP		0x05
#define ORA_ZP_X	0x15
#define ORA_ABS		0x0D
#define ORA_ABS_X	0x1D
#define ORA_ABS_Y	0x19
#define ORA_IND_X	0x01
#define ORA_IND_Y	0x11

//-----------------------------------------------------------------------------
// Bit test instructions 
//-----------------------------------------------------------------------------
#define BIT_ZP		0x24
#define BIT_ABS		0x2C

//-----------------------------------------------------------------------------
// Arithmetic instructions
//-----------------------------------------------------------------------------

// Add with carry instructions
#define ADC_IMM		0x69
#define ADC_ZP		0x65
#define ADC_ZP_X	0x75
#define ADC_ABS		0x6D
#define ADC_ABS_X	0x7D
#define ADC_ABS_Y	0x79
#define ADC_IND_X	0x61
#define ADC_IND_Y	0x71

// Subtract with carry instructions
#define SBC_IMM		0xE9
#define SBC_ZP		0xE5
#define SBC_ZP_X	0xF5
#define SBC_ABS		0xED
#define SBC_ABS_X	0xFD
#define SBC_ABS_Y	0xF9
#define SBC_IND_X	0xE1
#define SBC_IND_Y	0xF1

//-----------------------------------------------------------------------------
// Compare instructions
//-----------------------------------------------------------------------------
#define CMP_IMM		0xC9
#define CMP_ZP		0xC5
#define CMP_ZP_X	0xD5
#define CMP_ABS		0xCD
#define CMP_ABS_X	0xDD
#define CMP_ABS_Y	0xD9
#define CMP_IND_X	0xC1
#define CMP_IND_Y	0xD1

// Compare X Register instructions
#define CPX_IMM		0xE0
#define CPX_ZP		0xE4
#define CPX_ABS		0xEC

// Compare Y Register instructions
#define CPY_IMM		0xC0
#define CPY_ZP		0xC4
#define CPY_ABS		0xCC

//-----------------------------------------------------------------------------
// Increment and Decrement instructions
//-----------------------------------------------------------------------------

// Increment memory location instructions
#define INC_ZP		0xE6
#define INC_ZP_X	0xF6
#define INC_ABS		0xEE
#define INC_ABS_X	0xFE

// Increment X register instructions
#define INX			0xE8

// Increment Y register instructions
#define INY			0xC8

// Decrement memory location instructions
#define DEC_ZP		0xC6
#define DEC_ZP_X	0xD6
#define DEC_ABS		0xCE
#define DEC_ABS_X	0xDE

// Decrement X register instructions
#define DEX			0xCA

// Decrement Y register instructions
#define DEY			0x88

//-----------------------------------------------------------------------------
// Shift instructions
//-----------------------------------------------------------------------------

// Arithmetic shift left instructions
#define ASL_ACC		0x0A
#define ASL_ZP		0x06
#define ASL_ZP_X	0x16
#define ASL_ABS		0x0E
#define ASL_ABS_X	0x1E

// Logical shift right instructions
#define LSR_ACC		0x4A
#define LSR_ZP		0x46
#define LSR_ZP_X	0x56
#define LSR_ABS		0x4E
#define LSR_ABS_X	0x5E

// Rotate left instructions
#define ROL_ACC		0x2A
#define ROL_ZP		0x26
#define ROL_ZP_X	0x36
#define ROL_ABS		0x2E
#define ROL_ABS_X	0x3E

// Rotate right instructions
#define ROR_ACC		0x6A
#define ROR_ZP		0x66
#define ROR_ZP_X	0x76
#define ROR_ABS		0x6E
#define ROR_ABS_X	0x7E

//-----------------------------------------------------------------------------
// Jump and call instructions
//-----------------------------------------------------------------------------

// Jump instructions
#define JMP_ABS		0x4C
#define JMP_IND		0x6C

// Jump to subroutine instructions
#define JSR			0x20

// return from subroutine instructions
#define RTS			0x60

//-----------------------------------------------------------------------------
// Branch instructions
//-----------------------------------------------------------------------------

// Branch if carry clear
#define BCC			0x90

// Branch if carry set
#define BCS			0xB0

// Branch if equal
#define BEQ			0xF0

// Branch if minus
#define BMI			0x30

// Branch if not equal
#define BNE			0xD0

// Branch if positive
#define BPL			0x10

// Branch if overflow clear
#define BVC			0x50

// Branch if overflow set
#define BVS			0x70

//-----------------------------------------------------------------------------
// Status flag change instructions
//-----------------------------------------------------------------------------

// Clear carry flag
#define CLC			0x18

// Clear decimal mode flag
#define CLD			0xD8

// Clear interrupt disable flag
#define CLI			0x58

// Clear overflow flag
#define CLV			0xB8

// Set carry flag
#define SEC			0x38

// Set decimal mode flag
#define SED			0xF8

// Set interrupt disable flag
#define SEI			0x78

//-----------------------------------------------------------------------------
// System Functions
//-----------------------------------------------------------------------------

// Force an interrupt
#define BRK			0x00

// No operation instruction
#define NOP			0xEA

// Return from interrupt
#define RTI			0x40

// General purpose registers
static	u8 	A;
static	u8 	X;
static	u8 	Y;

// Special purpose registers
static	u8 	sp;
static	u8 	status;
static	u16 pc;

static	u8 memory[0x10000]; // for now allocate the entire address space for th emulator

static	const s32 CYCLES_PER_FRAME = 29834;
static	s32 remainingCycles; //borrowed from github/AndreaOrru/LaiNES 




/**
 * Performs the load accumulator instruction with the given addressing mode.
 *
 * @param mode: The addressing mode function, used to get the operand for
 * the store instruction.
 */
void lda_op(address_mode mode)
{
	A = memory[mode()];
	set_zero(A);
	set_negative(A);
}

void ldx_op(address_mode mode)
{
	X = memory[mode()];
	set_zero(X);
	set_negative(X);
}

void ldy_op(address_mode mode)
{
	Y = memory[mode()];
	set_zero(Y);
	set_negative(Y);
}

void sta_op(address_mode mode)
{
	memory[mode()] = A;
}

void stx_op(address_mode mode)
{
	memory[mode()] = X;
}

void sty_op(address_mode mode)
{
	memory[mode()] = Y;
}

//--------------------------------------------------------------------------
// Register Transfer instructions
//--------------------------------------------------------------------------
void tax_op(void)
{
	X = A;
    set_zero(X);
    set_negative(Y);
    pc++;
}

void tay_op(void)
{
    Y = A;
    set_zero(Y);
    set_negative(Y);
    pc++;
}

void txa_op(void)
{
    A = X;
    set_zero(A);
    set_negative(A);
    pc++;
}

void tya_op(void)
{
	A = Y;
    set_zero(A);
    set_negative(A);
    pc++;
}

//--------------------------------------------------------------------------
// Stack operations
//--------------------------------------------------------------------------

void tsx_op(void)
{
	X = sp;
    set_zero(X);
    set_negative(Y);
    pc++;
}

void txs_op(void)
{
	sp = X;    
    pc++;
}

void pha_op(void)
{
    memory[0x0100 + sp] = A;
    sp--;
    pc++;
}

void php_op(void)
{
    memory[0x0100 + sp] = status;
    sp--;
    pc++;
}

void pla_op(void)
{
    sp++;
    A = memory[0x0100 + sp];
    set_zero(A);
    set_negative(A);
    pc++;
}

void plp_op(void)
{
    sp++;
    status = memory[0x0100 + sp];
    pc++;
}

//-----------------------------------------------------------------------------
// Logical operations
//-----------------------------------------------------------------------------
void and_op(address_mode mode)
{
    A &= memory[mode()];
    set_zero(A);
    set_negative(A);
}

void eor_op(address_mode mode)
{
    A ^= memory[mode()];
    set_zero(A);
    set_negative(A);
}

void ora_op(address_mode mode)
{
    A |= memory[mode()];
    set_zero(A);
    set_negative(A);
}

void bit_op(address_mode mode)
{
    u8 temp = A & memory[mode()];
    set_zero(temp);
    set_overflow(temp);
    set_negative(temp);
}

//-----------------------------------------------------------------------------
// Arithmetic instructions
//-----------------------------------------------------------------------------
/**
 * Performs the add with carry instruction with the given addressing mode.
 *
 * @param mode: The addressing mode function, used to get the operand for
 * the addition.
 */
void adc_op(address_mode mode)
{
	u8 preAdd = A;
	u8 carry = status & CARRY_FLAG;
	A += memory[mode()] + carry;
	set_carry(preAdd);
	set_zero(A);
	set_overflow(A);
    set_negative(A);
}


void sbc_op(address_mode mode)
{
	u8 pre_sub = A;
    u8 carry = status & CARRY_FLAG;
    A -= memory[mode()] - (1 - carry);
    set_carry(pre_sub);
	set_zero(A);
	set_overflow(A);
    set_negative(A);
}

	
//-----------------------------------------------------------------------------
// Compare instructions
//-----------------------------------------------------------------------------
void cmp_op(address_mode mode)
{
    if (A >= memory[mode()]) {
        status |= CARRY_FLAG;
    }
    set_zero(A);
    set_negative(A);
}

void cpx_op(address_mode mode)
{
    if (X >= memory[mode()]) {
        status |= CARRY_FLAG;
    }
    set_zero(X);
    set_negative(X);
}

void cpy_op(address_mode mode)
{
    if (Y >= memory[mode()]) {
        status |= CARRY_FLAG;
    }
    set_zero(Y);
    set_negative(Y);
}

//-----------------------------------------------------------------------------
// Increment and Decrement instructions
//-----------------------------------------------------------------------------
void inc_op(address_mode mode)
{
    memory[mode()] += 1;
    set_zero(memory[mode()]);
    set_negative(memory[mode()]);
}

void inx_op(void)
{
    X++;
    set_zero(X);
    set_negative(X);
    pc++;
}

void iny_op(void)
{
    Y++;
    set_zero(Y);
    set_negative(Y);
    pc++;
}

void dec_op(address_mode mode)
{
    memory[mode()] -= 1;
    set_zero(memory[mode()]);
    set_negative(memory[mode()]);
}

void dex_op(void)
{
    X--;
    set_zero(X);
    set_negative(X);
    pc++;
}

void dey_op(void)
{
    Y--;
    set_zero(Y);
    set_negative(Y);
    pc++;
}

//-----------------------------------------------------------------------------
// Shift instructions
//-----------------------------------------------------------------------------

// Just gonna implement the accumulator version in the run() function
void asl_op(address_mode mode)
{
    u8 val = memory[mode()];
    // set carry flag
    status |= (NEGATIVE_FLAG & val) >> 7;
    val <<= 1;
    set_negative(val);
    set_zero(val);
    memory[mode()] = val;
}

void lsr_op(address_mode mode)
{
    u8 val = memory[mode()];
    // set carry flag
    status |= CARRY_FLAG & val;
    val >>= 1;
    set_negative(val);
    memory[mode()] = val;
}

void rol_op(address_mode mode)
{
	u8 val = memory[mode()];    
	u8 new_carry = val & 0x08;
	val = (status & CARRY_FLAG) << 1;
	set_carry(new_carry);
	set_negative(val);
	set_zero(val);
	memory[mode()] = val;
}

void ror_op(address_mode mode)
{
	u8 val = memory[mode()];    
	u8 new_carry = val & 0x08;
	val = (status & CARRY_FLAG) >> 1;
	set_carry(new_carry);
	set_negative(val);
	set_zero(val);
	memory[mode()] = val;
}

//-----------------------------------------------------------------------------
// Jump and call instructions
//-----------------------------------------------------------------------------
void jmp_op(address_mode mode)
{
    pc = memory[mode()];        
}

void jsr_op(void)
{
    memory[0x0100 + sp] = memory[get_absolute()] - 1;
    sp--;
}

void rts_op(void)
{
    sp++;
    pc = memory[0x0100 + sp];
    pc += 6;
}

//-----------------------------------------------------------------------------
// Branch instructions
//-----------------------------------------------------------------------------
void bcc_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, !(status & CARRY_FLAG)); 
}

void bcs_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, (status & CARRY_FLAG)); 
}

void beq_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, (status & ZERO_FLAG)); 
}

void bmi_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, (status & NEGATIVE_FLAG)); 
}

void bne_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, !(status & ZERO_FLAG)); 
    printf("%d\n", !(status & ZERO_FLAG));
}

void bpl_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, !(status & NEGATIVE_FLAG)); 
}

void bvc_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, !(status & OVERFLOW_FLAG)); 
}

void bvs_op(void)
{
    s8 displacement = static_cast<s8>(get_immediate());    
    do_branch(displacement, (status & OVERFLOW_FLAG)); 
}

//-----------------------------------------------------------------------------
// Status flag change instructions
//-----------------------------------------------------------------------------
void clc_op(void)
{
    status &= ~CARRY_FLAG;    
    pc++;
}

void cld_op(void)
{
    status &= ~DECIMAL_MODE_FLAG;    
    pc++;
}

void cli_op(void)
{
    status &= ~INTERRUPT_DISSABLE_FLAG;
    pc++;
}

void clv_op(void)
{
    status &= ~OVERFLOW_FLAG;
    pc++;
}

void sec_op(void)
{
    status |= CARRY_FLAG;
    pc++;
}

void sed_op(void)
{
    status |= DECIMAL_MODE_FLAG;
    pc++;
}

void sei_op(void)
{
    status |= INTERRUPT_DISSABLE_FLAG;
    pc++;
}

//-----------------------------------------------------------------------------
// System Functions
//-----------------------------------------------------------------------------
void brk_op(void)
{
    status |= BREAK_FLAG;
    memory[sp + 0x0100] = static_cast<u8>(0x0F & pc);
    sp--;
    memory[sp + 0x0100] = static_cast<u8>((0xF0 & pc) >> 8);
    sp--;
    memory[sp + 0x0100] = status;
    sp--;
    // Load the IRQ vector
    pc = memory[0xFFFE] << 8;
    pc |= memory[0xFFF];
}

void nop_op(void)
{
	pc++;
	tick();
	tick();
}

void rti_op(void)
{
    sp++;
    status = memory[sp + 0x0100];
    sp++;
    pc = memory[sp + 0x0100] << 8;
    sp++;
    pc |= memory[sp + 0x0100];
}

void init(void)
{
	A = 0;
	X = 0;
	Y = 0;
	//TODO: set SP and PC
	status = 0;
	sp = 0xff;
	remainingCycles = CYCLES_PER_FRAME;
}

void step(void)
{
	switch (memory[pc]) {

    case LDA_IMM:
        lda_op(get_immediate);
        tick2();
        break;
    case LDA_ZP:
        lda_op(get_zero_page);
        tick3();
        break;
    case LDA_ZP_X:
        lda_op(get_zero_page_x);
        tick4();
        break;
    case LDA_ABS:
        lda_op(get_absolute);
        tick4();
        break;
    case LDA_ABS_X:
        lda_op(get_absolute_X_index);
        tick4();
        break;
    case LDA_ABS_Y:
        lda_op(get_absolute_Y_index);
        tick4();
        break;
    case LDA_IND_X:
        lda_op(get_indexed_indirect);
        tick6();
        break;
    case LDA_IND_Y:
        lda_op(get_indirect_indexed);
        tick5();
        break;

    case LDX_IMM:
        ldx_op(get_immediate);
        tick2();
        break;
    case LDX_ZP:
        ldx_op(get_zero_page);
        tick3();
        break;
    case LDX_ZP_Y:
        ldx_op(get_zero_page_y);
        tick4();
        break;
    case LDX_ABS:
        ldx_op(get_absolute);
        tick4();
        break;
    case LDX_ABS_Y:
        ldx_op(get_absolute_Y_index);
        tick4();
        break;


    case LDY_IMM: 
        ldy_op(get_immediate);
        tick2();
        break;
    case LDY_ZP:	 
        ldy_op(get_zero_page);
        tick3();
        break;
    case LDY_ZP_X: 
        ldy_op(get_zero_page_x);
        tick4();
        break;
    case LDY_ABS:	 
        ldy_op(get_absolute);
        tick4();
        break;
    case LDY_ABS_X:
        ldy_op(get_absolute_X_index);
        tick4();
        break;

    case STA_ZP:
        sta_op(get_zero_page);
        tick3();
        break;
    case STA_ZP_X:
        sta_op(get_zero_page_x);
        tick4();
        break;
    case STA_ABS:	 
        sta_op(get_absolute);
        tick4();
        break;
    case STA_ABS_X:
        sta_op(get_absolute_X_index);
        tick5();
        break;
    case STA_ABS_Y:
        sta_op(get_absolute_Y_index);
        tick5();
        break;
    case STA_IND_X:
        sta_op(get_indexed_indirect);
        tick6();
        break;
    case STA_IND_Y:
        sta_op(get_indirect_indexed);
        tick6();
        break;

    case STX_ZP:	
        stx_op(get_zero_page);
        tick3();
        break;
    case STX_ZP_Y:
        stx_op(get_zero_page_y);
        tick4();
        break;
    case STX_ABS:	
        stx_op(get_absolute);
        tick4();
        break;

    case STY_ZP:	
        sty_op(get_zero_page);
        tick3();
        break;
    case STY_ZP_X:
        sty_op(get_zero_page_x);
        tick4();
        break;
    case STY_ABS:	
        sty_op(get_absolute);
        tick4();
        break;

    case TAX:
        tax_op();
        tick2();
        break;
    case TAY:
        tay_op();
        tick2();
        break;
    case TXA:
        txa_op();
        tick2();
        break;
    case TYA:
        tya_op();
        tick2();
        break;

    case TSX:
        tsx_op();
        tick2();
        break;
    case TXS:
        txs_op();
        tick2();
        break;
    case PHA:
        pha_op();
        tick3();
        break;
    case PHP:
        php_op();
        tick3();    
        break;
    case PLA:
        pla_op();
        tick4();
        break;
    case PLP:
        plp_op();
        tick4();
        break;

    case AND_IMM:	 
        and_op(get_immediate);
        tick2();
        break;
    case AND_ZP:	 
        and_op(get_zero_page);
        tick3();
        break;
    case AND_ZP_X: 
        and_op(get_zero_page_x);
        tick4();
        break;
    case AND_ABS:	 
        and_op(get_absolute);
        tick4();
        break;
    case AND_ABS_X:
        and_op(get_absolute_X_index);
        tick4();
        break;
    case AND_ABS_Y:
        and_op(get_absolute_Y_index);
        tick4();
        break;
    case AND_IND_X:
        and_op(get_indexed_indirect);
        tick6();
        break;
    case AND_IND_Y:
        and_op(get_indirect_indexed);
        tick5();
        break;

    case EOR_IMM:	 
        eor_op(get_immediate);
        tick2();
        break;
    case EOR_ZP:	 
        eor_op(get_zero_page);
        tick3();
        break;
    case EOR_ZP_X: 
        eor_op(get_zero_page_x);
        tick4();
        break;
    case EOR_ABS:	 
        eor_op(get_absolute);
        tick4();
        break;
    case EOR_ABS_X:
        eor_op(get_absolute_X_index);
        tick4();
        break;
    case EOR_ABS_Y:
        eor_op(get_absolute_Y_index);
        tick4();
        break;
    case EOR_IND_X:
        eor_op(get_indexed_indirect);
        tick6();
        break;
    case EOR_IND_Y:
        eor_op(get_indirect_indexed);
        tick5();
        break;

    case ORA_IMM:	 
        ora_op(get_immediate);
        tick2();
        break;
    case ORA_ZP:	 
        ora_op(get_zero_page);
        tick3();
        break;
    case ORA_ZP_X: 
        ora_op(get_zero_page_x);
        tick4();
        break;
    case ORA_ABS:	 
        ora_op(get_absolute);
        tick4();
        break;
    case ORA_ABS_X:
        ora_op(get_absolute_X_index);
        tick4();
        break;
    case ORA_ABS_Y:
        ora_op(get_absolute_Y_index);
        tick4();
        break;
    case ORA_IND_X:
        ora_op(get_indexed_indirect);
        tick6();
        break;
    case ORA_IND_Y:
        ora_op(get_indirect_indexed);
        tick5();
        break;


    case BIT_ZP: 
        bit_op(get_zero_page);
        tick3();
        break;
    case BIT_ABS:
        bit_op(get_absolute);
        tick4();
        break;

	case ADC_IMM:
		adc_op(get_immediate);
		tick2();
		break;

	case ADC_ZP:
		adc_op(get_zero_page);
		tick3();
		break;

	case ADC_ZP_X:
		adc_op(get_zero_page_x);
		tick4();
		break;

	case ADC_ABS:
		adc_op(get_absolute);
		tick4();
		break;

	case ADC_ABS_X:
		adc_op(get_absolute_X_index);
		tick4();
		break;

	case ADC_ABS_Y:
		adc_op(get_absolute_Y_index);
		tick4();
		break;

	case ADC_IND_X:
		adc_op(get_indexed_indirect);
		tick6();
		break;

	case ADC_IND_Y:
		adc_op(get_indirect_indexed);
		tick5();
		break;

    case SBC_IMM:	 
        sbc_op(get_immediate);
        tick2();
        break;
    case SBC_ZP:	 
        sbc_op(get_zero_page);
        tick3();
        break;
    case SBC_ZP_X: 
        sbc_op(get_zero_page_x);
        tick4();
        break;
    case SBC_ABS:	 
        sbc_op(get_absolute);
        tick4();
        break;
    case SBC_ABS_X:
        sbc_op(get_absolute_X_index);
        tick4();
        break;
    case SBC_ABS_Y:
        sbc_op(get_absolute_Y_index);
        tick4();
        break;
    case SBC_IND_X:
        sbc_op(get_indexed_indirect);
        tick6();
        break;
    case SBC_IND_Y:
        sbc_op(get_indirect_indexed);
        tick5();
        break;

    case CMP_IMM:	 
        sbc_op(get_immediate);
        tick2();
        break;
    case CMP_ZP:	 
        sbc_op(get_zero_page);
        tick3();
        break;
    case CMP_ZP_X: 
        sbc_op(get_zero_page_x);
        tick4();
        break;
    case CMP_ABS:	 
        sbc_op(get_absolute);
        tick4();
        break;
    case CMP_ABS_X:
        sbc_op(get_absolute_X_index);
        tick4();
        break;
    case CMP_ABS_Y:
        sbc_op(get_absolute_Y_index);
        tick4();
        break;
    case CMP_IND_X:
        sbc_op(get_indexed_indirect);
        tick6();
        break;
    case CMP_IND_Y:
        sbc_op(get_indirect_indexed);
        tick5();
        break;

    case CPX_IMM:
        cpx_op(get_immediate);
        tick2();
        break;
    case CPX_ZP: 
        cpx_op(get_zero_page);
        tick3();
        break;
    case CPX_ABS:
        cpx_op(get_absolute);
        tick4();
        break;

    case CPY_IMM:
        cpy_op(get_immediate);
        tick2();
        break;
    case CPY_ZP: 
        cpy_op(get_zero_page);
        tick3();
        break;
    case CPY_ABS:
        cpy_op(get_absolute);
        tick4();
        break;

    case INC_ZP:	 
        inc_op(get_zero_page);
        tick5();
        break;
    case INC_ZP_X: 
        inc_op(get_zero_page_x);
        tick6();
        break;
    case INC_ABS:	 
        inc_op(get_absolute);
        tick6();
        break;
    case INC_ABS_X:
        inc_op(get_absolute_X_index);
        tick7();
        break;

    case INX:
        inx_op();
        tick2();
        break;

    case INY:
        iny_op();
        tick2();
        break;

    case DEC_ZP:	 
        dec_op(get_zero_page);
        tick5();
        break;
    case DEC_ZP_X: 
        dec_op(get_zero_page_x);
        tick6();
        break;
    case DEC_ABS:	 
        dec_op(get_absolute);
        tick6();
        break;
    case DEC_ABS_X:
        dec_op(get_absolute_X_index);
        tick7();
        break;

    case DEX:
        dex_op();
        tick2();
        break;

    case DEY:
        dey_op();
        tick2();
        break;

    case ASL_ACC:	 
        asl_op(get_accumulator);
        tick2();
        break;
    case ASL_ZP:	 
        asl_op(get_zero_page);
        tick5();
        break;
    case ASL_ZP_X: 
        asl_op(get_zero_page_x);
        tick6();
        break;
    case ASL_ABS:	 
        asl_op(get_absolute);
        tick6();
        break;
    case ASL_ABS_X:
        asl_op(get_absolute_X_index);
        tick7();
        break;

    case LSR_ACC:	 
        lsr_op(get_accumulator);
        tick2();
        break;
    case LSR_ZP:	 
        lsr_op(get_zero_page);
        tick5();
        break;
    case LSR_ZP_X: 
        lsr_op(get_zero_page_x);
        tick6();
        break;
    case LSR_ABS:	 
        lsr_op(get_absolute);
        tick6();
        break;
    case LSR_ABS_X:
        lsr_op(get_absolute_X_index);
        tick7();
        break;

    case ROL_ACC:	 
        rol_op(get_accumulator);
        tick2();
        break;
    case ROL_ZP:	 
        rol_op(get_zero_page);
        tick5();
        break;
    case ROL_ZP_X: 
        rol_op(get_zero_page_x);
        tick6();
        break;
    case ROL_ABS:	 
        rol_op(get_absolute);
        tick6();
        break;
    case ROL_ABS_X:
        rol_op(get_absolute_X_index);
        tick7();
        break;

    case ROR_ACC:	 
        ror_op(get_accumulator);
        tick2();
        break;
    case ROR_ZP:	 
        ror_op(get_zero_page);
        tick5();
        break;
    case ROR_ZP_X: 
        ror_op(get_zero_page_x);
        tick6();
        break;
    case ROR_ABS:	 
        ror_op(get_absolute);
        tick6();
        break;
    case ROR_ABS_X:
        ror_op(get_absolute_X_index);
        tick7();
        break;

    case JMP_ABS:
        jmp_op(get_absolute);
        tick3();
        break;
    case JMP_IND:
        jmp_op(get_indirect);
        tick5();
        break;

    case JSR:
        jsr_op();
        tick6();
        break;

    case RTS:
        rts_op();
        tick6();
        break;

    case BCC:
        bcc_op();
        tick2();
        break;

    case BCS:
        bcs_op();
        tick2();
        break;

    case BEQ:
        beq_op();
        tick2();
        break;

    case BMI:
        bmi_op();
        tick2();
        break;

    case BNE:
        bne_op();
        tick2();
        break;

    case BPL:			
        bpl_op();
        tick2();
        break;

    case BVC:
        bvc_op();
        tick2();
        break;

    case BVS:
        bvs_op();
        tick2();
        break;

    case CLC:
        clc_op();
        tick2();
        break;

    case CLD:
        cld_op();
        tick2();
        break;

    case CLI:
        cli_op();
        tick2();
        break;

    case CLV:
        clv_op();
        tick2();
        break;

    case SEC:
        sec_op();
        tick2();
        break;

    case SED:
        sed_op();
        tick2();
        break;

    case SEI:
        sei_op();
        tick2();
        break;

    case BRK:
        brk_op();
        tick7();
        break;

    case RTI:
        rti_op();
        tick6();
        break;

    case NOP:
        nop_op();
        break;
	
    default:
        break;
	}
}


u8 get_regA(void)
{
    return A;
}

u8 get_regX(void)
{
    return X;
}

u8 get_regY(void)
{
    return Y;
}

u8 get_status(void)
{
    return status;
}

u8 get_sp(void)
{
    return sp;
}

u16 get_pc(void)
{
    return pc;
}


void testCpu(const std::vector<u8> &code) 
{
	memcpy(memory, code.data(), code.size());
	while (pc < code.size()) {
		printf("___________________________________________\n");
		printf("PC: 0x%X | op_code: 0x%X\n", pc, memory[pc]);
		printf("A: 0x%X | X: 0x%X | Y: 0x%X | sp: 0x%X\n", A, X, Y, sp);
		printf("___________________________________________\n");

		step();
	}
		printf("___________________________________________\n");
		printf("PC: 0x%X | op_code: 0x%X\n", pc, memory[pc]);
		printf("A: 0x%X | X: 0x%X | Y: 0x%X | sp: 0x%X\n", A, X, Y, sp);
		printf("___________________________________________\n");
}

/**
 * Run the Cpu for a cycle.
 */
void tick(void)
{
	// TODO: call ppu three times since it runs at 3 x the 6502 clock speed
	// NOTE: the cpu runs at 1.79 MHz which comes down to roughly 29834 cycles
	// per frame
	remainingCycles--;
}


// memory access methods
// All memory access methods grab the address from the opCode's operands.
// In otherwords, once an opcode is read the memory methods will increment
// the program counter and then use the new value in the program counter
// to get the value that will be used for accessing memory.

u16 get_accumulator(void)
{
    return A;
}

/**
 * Grabs the immediate value from the next byte in program memory.
 *
 * @return: The value found at the next byte.
 */
u16 get_immediate(void)
{
	Cpu::pc++;
	u16 ret = pc;
	Cpu::pc++;
	return ret;
}

/**
 * Grabs the next byte from program memory and uses that to retrieve a 
 * value from the zero page.
 *
 * @return: The value found in the zero page.
 */
u16 get_zero_page(void)
{
	return get_immediate();
}

/**
 * Grabs the next byte from program memory and adds that to the value found
 * in the X register and uses that new value to retrieve a value from the
 * zero page.
 *
 * @return: The value found in the zero page.
 */
u16 get_zero_page_x(void)
{
	return get_immediate() + X;
}

u16 get_zero_page_y(void)
{
    return get_immediate() + Y;
}

/**
 * Grabs the 16 bit address from the next two bytes and uses that to access
 * a value in memory.
 *
 * @return: The value found at the absolute address.
 */
u16 get_absolute(void)
{
	pc++;
	u16 loc = memory[pc] << 8;
	pc++;
	loc |= memory[pc];
	pc++;

	return loc;
}

/**
 * Grabs the 16 bit address from the next two bytes and adds it to the 
 * value found in the X register and uses the calculated value to fetch a
 * value from memory.
 *
 * @return: The value found in memory.
 */
u16 get_absolute_X_index(void)
{
	// TODO: refractor out the duplicate code
	pc++;
	u16 loc = memory[pc] << 8;
	pc++;
	loc |= memory[pc];
	pc++;

    if ((loc & 0x00FF) + X > 0x00FF) {
        tick();	// handle page cross
    }

	return loc + X;
}

/**
 * Grabs the 16 bit address from the next two bytes and adds it to the 
 * value found in the y register and uses the calculated value to fetch a
 * value from memory.
 *
 * @return: The value found in memory.
 */
u16 get_absolute_Y_index(void)
{
	// TODO: refractor out the duplicate code
	pc++;
	u16 loc = memory[pc];
	pc++;
	loc |= memory[pc] << 8;
	pc++;

    if ((loc & 0x00FF) + Y > 0x00FF) {
        tick();	// handle page cross
    }

	return loc + Y;
}

u16 get_indirect(void)
{
    u16 loc = get_absolute();
    u16 dest = memory[loc];
    dest |= memory[loc + 1] << 8;
    return dest;
}

/**
 * The value in the X register is added to the next byte. This result is
 * used to grab a 16 bit value from the zero page which is then used to
 * access a value in memory.
 *
 * @return: The value found in memory.
 */
u16 get_indexed_indirect(void)
{
	u8 zp = get_immediate() + X;
	u16 ret = memory[zp];
	ret |= memory[zp + 1] << 8;
	return ret;
}

/**
 * Takes the value in the next byte, uses that to retrieve a 16 bit value
 * from the zero page and adds the value in the Y register to that. The 
 * result is then used to access a value in memory.
 *
 * @return: The value found in memory.
 */
u16 get_indirect_indexed(void)
{
	u8 zp = get_immediate();
	u16 address = memory[zp] << 8;
	address |= memory[zp + 1];
	address += Y;
    
    if ((address & 0x00FF) + Y > 0x00FF) {
        tick();	// handle page cross
    }

    return address;
}

// TODO: consider converting these to macros
// Flag operations
void set_carry(u8 preAdd)
{
    // NOTE: this probably only works for addition, also there may be a
    // simpler method to do this
	if (preAdd > A) {
        status |= CARRY_FLAG;
    }
}

void set_zero(u8 reg)
{
    if (!reg) {
        status |= ZERO_FLAG;
    }
}

void set_overflow(u8 reg)
{
    if ((status & CARRY_FLAG) && (reg & NEGATIVE_FLAG)) {
        status |= OVERFLOW_FLAG;
    }
}

void set_negative(u8 reg)
{
	if (reg & NEGATIVE_FLAG) {
		status |= NEGATIVE_FLAG;
	}
}

void new_page_cycle(u16 old_pc)
{
    if (pc - old_pc > 100) {
        printf("%d - %d = %d\n", old_pc + 100, pc,(old_pc + 100) - pc);
        tick2();
    }
}

//TODO: have to redo how the displacement works
void do_branch(s8 displacement, bool exp)
{
    u16 old_pc = pc;
        printf("%d\n", pc);
    if (exp) {
        pc = displacement;
        printf("%d\n", pc);
        tick();
        new_page_cycle(old_pc);
    }
}
}
