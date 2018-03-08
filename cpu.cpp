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

// Load/Store Operations
#define LDA_IMM		0xA9
#define LDA_ZP		0xA5
#define LDA_ZP_X	0xB5
#define LDA_ABS		0xAD
#define LDA_ABS_X	0xBD
#define LDA_ABS_Y	0xB9
#define LDA_IND_X	0xA1
#define LDA_IND_Y	0xB1

#define LDX_IMM		0xA2
#define LDX_ZP		0xA6
#define LDX_ZP_Y	0xB6
#define LDX_ABS		0xAE
#define LDX_ABS_Y	0xBE

#define LDY_IMM		0xA0
#define LDY_ZP		0xA4
#define LDY_ZP_X	0xB4
#define LDY_ABS		0xAC
#define LDY_ABS_X	0xBC

#define STA_ZP		0x85
#define STA_ZP_X	0x95
#define STA_ABS		0x8D
#define STA_ABS_X	0x9D
#define STA_ABS_Y	0x99
#define STA_IND_X	0x81
#define STA_IND_Y	0x91

#define STX_ZP		0x86
#define STX_ZP_Y	0x96
#define STX_ABS		0x8E

#define STY_ZP		0x84
#define STY_ZP_X	0x94
#define STY_ABS		0x8C

// Arithmetic instructions

// Add with carry instructions
#define ADC_IMM		0x69
#define ADC_ZP		0x65
#define ADC_ZP_X	0x75
#define ADC_ABS		0x6D
#define ADC_ABS_X	0x7D
#define ADC_ABS_Y	0x79
#define ADC_IND_X	0x61
#define ADC_IND_Y	0x71

#define NOP			0xEA

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
		addc(getImmediate);
		tick2();
		break;

	case ADC_ZP:
		addc(getZeroPage);
		tick3();
		break;

	case ADC_ZP_X:
		addc(getZeroPageIndexed);
		tick4();
		break;

	case ADC_ABS:
		addc(getAbsolute);
		tick4();
		break;

	case ADC_ABS_X:
		addc(getAbsoluteXIndex);
		tick4();
		break;

	case ADC_ABS_Y:
		addc(getAbsoluteYIndex);
		tick4();
		break;

	case ADC_IND_X:
		addc(getIndexedIndirect);
		tick6();
		break;

	case ADC_IND_Y:
		addc(getIndirectIndexed);
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
u8 getImmediate(void)
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
u8 getZeroPage(void)
{
	return getImmediate();
}

/**
 * Grabs the next byte from program memory and adds that to the value found
 * in the X register and uses that new value to retrieve a value from the
 * zero page.
 *
 * @return: The value found in the zero page.
 */
u8 getZeroPageIndexed(void)
{
	return memory[getImmediate() + X];
}

/**
 * Grabs the 16 bit address from the next two bytes and uses that to access
 * a value in memory.
 *
 * @return: The value found at the absolute address.
 */
u8 getAbsolute(void)
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
u8 getAbsoluteXIndex(void)
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
u8 getAbsoluteYIndex(void)
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
u8 getIndexedIndirect(void)
{
	u8 zp = getImmediate() + X;
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
u8 getIndirectIndexed(void)
{
	u8 zp = getImmediate();
	u16 address = memory[zp] << 8;
	address |= memory[zp + 1];
	address += Y;
    
    if ((address & 0x00FF) + Y > 0x00FF) {
        tick();	// handle page cross
    }

    return memory[address];
}

// Flag operations
void setCarry(u8 preAdd)
{
    // NOTE: this probably only works for addition, also there may be a
    // simpler method to do this
	if (preAdd > A) {
        status |= CARRY;
    }
}

void setZero(void)
{
    if (!A) {
        status |= ZERO;
    }
}

void setOverflow(void)
{
    if ((status & CARRY) && (A & NEGATIVE)) {
        status |= OVERFLOW;
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

}

void ldx(address_mode mode)
{

}

void ldy(address_mode mode)
{

}

void sta(address_mode mode)
{

}

void stx(address_mode mode)
{

}

void sty(address_mode mode)
{

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
	setCarry(preAdd);
	setZero();
	setOverflow();
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


