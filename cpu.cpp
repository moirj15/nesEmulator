#include <string.h>

#include "cpu.h"

namespace Cpu {

// Time saving macros
#define tick2() tick();tick();
#define tick3() tick();tick();tick();
#define tick4() tick();tick();tick();tick();
#define tick5() tick();tick();tick();tick();tick();
#define tick6() tick();tick();tick();tick();tick();tick();
#define tick7() tick();tick();tick();tick();tick();tick();tick();

//------------------------------------------------------------------------------
// Load/Store Operations
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// Register Transfer instructions
//------------------------------------------------------------------------------
#define TAX			0xAA
#define TAY			0xA8
#define TXA			0x8A
#define TYA			0x98

//------------------------------------------------------------------------------
// Stack operations
//------------------------------------------------------------------------------
#define TSX			0xBA
#define TXS			0x9A
#define PHA			0x48
#define PHP			0x08
#define PLA			0x68
#define PLP			0x28

//------------------------------------------------------------------------------
// Logical operations
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// Bit test instructions 
//------------------------------------------------------------------------------
#define BIT_ZP		0x24
#define BIT_ABS		0x2C

//------------------------------------------------------------------------------
// Arithmetic instructions
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// Compare instructions
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Increment and Decrement instructions
//------------------------------------------------------------------------------

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
#define DEC_ZP_X	0xC6
#define DEC_ABS		0xCE
#define DEC_ABS_X	0xCE

// Decrement X register instructions
#define DEX			0xCA

// Decrement Y register instructions
#define DEY			0x88

//------------------------------------------------------------------------------
// Shift instructions
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// Jump and call instructions
//------------------------------------------------------------------------------

// Jump instructions
#define JMP_ABS		0x4C
#define JMP_IND		0x6C

// Jump to subroutine instructions
#define JSR			0x20

// return from subroutine instructions
#define RTS			0x60

//------------------------------------------------------------------------------
// Branch instructions
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// Status flag change instructions
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// System Functions
//------------------------------------------------------------------------------

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

static	u8 memory[0x10000]; // for now allocate the entire address space for the emulator

static	const s32 CYCLES_PER_FRAME = 29834;
static	s32 remainingCycles; //borrowed from github/AndreaOrru/LaiNES 


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


void run(void)
{
	switch (memory[pc]) {
    case NOP:
        nop();
        break;
	
	case ADC_IMM:
		addc(get_immediate);
		tick2();
		break;

	case ADC_ZP:
		addc(get_zero_page);
		tick3();
		break;

	case ADC_ZP_X:
		addc(get_zero_page_indexed);
		tick4();
		break;

	case ADC_ABS:
		addc(get_absolute);
		tick4();
		break;

	case ADC_ABS_X:
		addc(get_absolute_X_index);
		tick4();
		break;

	case ADC_ABS_Y:
		addc(get_absolute_Y_index);
		tick4();
		break;

	case ADC_IND_X:
		addc(get_indexed_indirect);
		tick6();
		break;

	case ADC_IND_Y:
		addc(get_indirect_indexed);
		tick5();
		break;

	}
}

void testCpu(u8 *code, u32 size) 
{
	memcpy(memory, code, size);
	while (pc < size) {
		printf("___________________________________________\n");
		printf("PC: %d | op_code: %X\n", pc, memory[pc]);
		printf("A: %d | X: %d | Y: %d | sp: %d\n", A, X, Y, sp);
		printf("___________________________________________\n");


		run();
	}
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

//TODO: write better method descriptions.

/**
 * Grabs the immediate value from the next byte in program memory.
 *
 * @return: The value found at the next byte.
 */
u8 get_immediate(void)
{
	Cpu::pc++;
	u8 ret = Cpu::memory[Cpu::pc];
	Cpu::pc++;
	return ret;
}

/**
 * Grabs the next byte from program memory and uses that to retrieve a 
 * value from the zero page.
 *
 * @return: The value found in the zero page.
 */
u8 get_zero_page(void)
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
u8 get_zero_page_indexed(void)
{
	return memory[get_immediate() + X];
}

/**
 * Grabs the 16 bit address from the next two bytes and uses that to access
 * a value in memory.
 *
 * @return: The value found at the absolute address.
 */
u8 get_absolute(void)
{
	pc++;
	u16 loc = memory[pc] << 8;
	pc++;
	loc |= memory[pc];
	pc++;

	return memory[loc];
}

/**
 * Grabs the 16 bit address from the next two bytes and adds it to the 
 * value found in the X register and uses the calculated value to fetch a
 * value from memory.
 *
 * @return: The value found in memory.
 */
u8 get_absolute_X_index(void)
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

	return memory[loc + X];
}

/**
 * Grabs the 16 bit address from the next two bytes and adds it to the 
 * value found in the y register and uses the calculated value to fetch a
 * value from memory.
 *
 * @return: The value found in memory.
 */
u8 get_absolute_Y_index(void)
{
	// TODO: refractor out the duplicate code
	pc++;
	u16 loc = memory[pc] << 8;
	pc++;
	loc |= memory[pc];
	pc++;

    if ((loc & 0x00FF) + Y > 0x00FF) {
        tick();	// handle page cross
    }

	return memory[loc + Y];
}

/**
 * The value in the X register is added to the next byte. This result is
 * used to grab a 16 bit value from the zero page which is then used to
 * access a value in memory.
 *
 * @return: The value found in memory.
 */
u8 get_indexed_indirect(void)
{
	u8 zp = get_immediate() + X;
	u16 ret = memory[zp] << 8;
	ret |= memory[zp + 1];
	return memory[ret];
}

/**
 * Takes the value in the next byte, uses that to retrieve a 16 bit value
 * from the zero page and adds the value in the Y register to that. The 
 * result is then used to access a value in memory.
 *
 * @return: The value found in memory.
 */
u8 get_indirect_indexed(void)
{
	u8 zp = get_immediate();
	u16 address = memory[zp] << 8;
	address |= memory[zp + 1];
	address += Y;
    
    if ((address & 0x00FF) + Y > 0x00FF) {
        tick();	// handle page cross
    }

    return memory[address];
}

// Flag operations
void set_carry(u8 preAdd)
{
    // NOTE: this probably only works for addition, also there may be a
    // simpler method to do this
	if (preAdd > A) {
        status |= CARRY;
    }
}

void set_zero(u8 reg)
{
    if (!reg) {
        status |= ZERO;
    }
}

void set_overflow(u8 reg)
{
    if ((status & CARRY) && (reg & NEGATIVE)) {
        status |= OVERFLOW;
    }
}

void set_negative(u8 reg)
{
	if (reg & NEGATIVE) {
		status |= NEGATIVE;
	}
}

/**
 * Performs the load accumulator instruction with the given addressing mode.
 *
 * @param mode: The addressing mode function, used to get the operand for
 * the store instruction.
 */
void lda(address_mode mode)
{
	A = mode();
	set_zero(A);
	set_negative(A);
}

void ldx(address_mode mode)
{
	X = mode();
	set_zero(X);
	set_negative(X);
}

void ldy(address_mode mode)
{
	Y = mode();
	set_zero(Y);
	set_negative(Y);
}

void sta(address_mode mode)
{
	memory[mode()] = A;
}

void stx(address_mode mode)
{
	memory[mode()] = X;
}

void sty(address_mode mode)
{
	memory[mode()] = Y;
}

/**
 * Performs the add with carry instruction with the given addressing mode.
 *
 * @param mode: The addressing mode function, used to get the operand for
 * the addition.
 */
void addc(address_mode mode)
{
	u8 preAdd = A;
	u8 carry = status & CARRY;
	A += mode() + carry;
	set_carry(preAdd);
	set_zero(A);
	set_overflow(A);
}


	
/**
 * No operation.
 */
void nop(void)
{
	pc++;
	tick();
	tick();
}













} // end Cpu namespace


