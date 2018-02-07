#include "cpu.h"

// Add with carry instructions
#define ADC_IMM		0x69
#define ADC_ZP		0x65
#define ADC_ZPX		0x75
#define ADC_ABS		0x6D
#define ADC_ABS_X	0x7D
#define ADC_ABS_Y	0x79
#define ADC_IND_X	0x61
#define ADC_IND_Y	0x71

#define NOP		0xEA

Cpu::Cpu(void)
{
	A = 0;
	X = 0;
	Y = 0;
	//TODO: set SP and PC
	status = 0;
	remainingCycles = CYCLES_PER_FRAME;
}

Cpu::~Cpu(void)
{

}

/**
 * Run the Cpu for a cycle.
 */
void Cpu::tick(void)
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
u8 Cpu::getImmediate(void)
{
	pc++;
	u8 ret = memory[pc];
	pc++;
	return ret;
}


/**
 * Grabs the next byte from program memory and uses that to retrieve a 
 * value from the zero page.
 *
 * @return: The value found in the zero page.
 */
u8 Cpu::getZeroPage(void)
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
u8 Cpu::getZeroPageIndexed(void)
{
	return memory[getImmediate() + X];
}

/**
 * Grabs the 16 bit address from the next two bytes and uses that to access
 * a value in memory.
 *
 * @return: The value found at the absolute address.
 */
u8 Cpu::getAbsolute(void)
{
	pc++;
	u16 ret = memory[pc] << 8;
	pc++;
	ret |= memory[pc];
	pc++;

	return ret;
}

/**
 * Grabs the 16 bit address from the next two bytes and adds it to the 
 * value found in the X register and uses the calculated value to fetch a
 * value from memory.
 *
 * @return: The value found in memory.
 */
u8 Cpu::getAbsoluteXIndex(void)
{
	return memory[getAbsolute() + X];
}

/**
 * Grabs the 16 bit address from the next two bytes and adds it to the 
 * value found in the y register and uses the calculated value to fetch a
 * value from memory.
 *
 * @return: The value found in memory.
 */
u8 Cpu::getAbsoluteYIndex(void)
{
	return memory[getAbsolute() + Y];
}

/**
 * The value in the X register is added to the next byte. This result is
 * used to grab a 16 bit value from the zero page which is then used to
 * access a value in memory.
 *
 * @return: The value found in memory.
 */
u8 Cpu::getIndexedIndirect(void)
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
u8 Cpu::getIndirectIndexed(void)
{

}


void Cpu::run(void)
{
	switch (memory[pc]) {
		case NOP:
			nop();
			break;
	}
}

/**
 * Add with carry instructions.
 */
void Cpu::addc_imm(void)
{
	pc++;
	u8 preAdd = A >> 7;
	A += memory[pc] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
}

void Cpu::addc_zp(void)
{
	addc_imm();
	tick();
}

void Cpu::addc_zpx(void)
{
	pc++;
	u8 preAdd = A >> 7;
	A += memory[memory[pc] + X] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;

	tick();
	tick();
	tick();
	tick();

}

void Cpu::addc_abs(void)
{
	pc++;
	u8 preAdd = A >> 7;
	u16 memLoc = memory[pc] << 8;
	pc++;
	memLoc |= memory[pc];
	A += memory[memLoc] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
	tick();
	tick();
}

void Cpu::addc_abs_x(void)
{
	pc++;
	u8 preAdd = A >> 7;
	u16 memLoc = memory[pc] << 8;
	pc++;
	memLoc |= memory[pc];
	A += memory[memLoc + X] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
	tick();
	tick();
	//TODO: check if page is crossed

}

void Cpu::addc_abs_y(void)
{
	pc++;
	u16 memLoc = memory[pc] << 8;
	memLoc |= memory[pc + 1];
	A += memory[memLoc] + (CARRY & status);
	pc++;
	tick();
	tick();
	tick();
	tick();
	//TODO: check if page is crossed
}

void Cpu::addc_ind_x(void)
{
	pc++;
	u16 memLoc = memory[memory[pc]] + X;
	memLoc |= memory[memory[pc]] + X + 1;
	A += memory[memLoc] + (CARRY & status);
	tick();
	tick();
	tick();
	tick();
	tick();
	tick();
	//TODO: check if page is crossed
}

void Cpu::addc_ind_y(void)
{
	pc++;
	u16 memLoc = memory[memory[pc]] << 8;
	memLoc |= memory[memory[pc] + 1];
	memLoc += Y;
	A += memory[memLoc] + (CARRY & status);
	pc++;
	tick();
	tick();
	tick();
	tick();
	tick();
	//TODO: check if page is crossed
}

	
/**
 * No operation.
 */
void Cpu::nop(void)
{
	pc++;
	tick();
	tick();
}
