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

void Cpu::run(void)
{
	switch (memory[pc]) {
    case NOP:
        nop();
        break;
	}
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
u8 Cpu::getAbsoluteXIndex(void)
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
u8 Cpu::getAbsoluteYIndex(void)
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
void Cpu::setCarry(u8 preAdd)
{
    // NOTE: this probably only works for addition, also there may be a
    // simpler method to do this
	if (preAdd > A) {
        status |= CARRY;
    }
}

void Cpu::setZero(void)
{
    if (!A) {
        status |= ZERO;
    }
}

void Cpu::setOverflow(u8 preAdd)
{
    if ((status & CARRY) && (A & NEGATIVE)) {
        status |= OVERFLOW;
    }
}

/**
 * Add with carry instructions.
 */
void Cpu::addc_imm(void)
{
	u8 preAdd = A;
	u8 carry = status & CARRY;
	A += getImmediate() + carry;
	setCarry(preAdd);
	setZero();
	setOverflow(preAdd);

	tick();
	tick();
}

void Cpu::addc_zp(void)
{
	u8 preAdd = A;
	u8 carry = status & CARRY;
	A += memory[getImmediate()] + carry;
	setCarry(preAdd);
	setZero();
	setOverflow(preAdd);

	tick();
	tick();
	tick();
}

void Cpu::addc_zpx(void)
{
	u8 preAdd = A;
    u8 carry = status & CARRY;
    A += getZeroPageIndexed() + carry;
	setCarry(preAdd);
	setZero();
	setOverflow(preAdd);

	tick();
	tick();
	tick();
	tick();
}

void Cpu::addc_abs(void)
{
	u8 preAdd = A;
    u8 carry = status & CARRY;
    A += getAbsolute() + carry;
	setCarry(preAdd);
	setZero();
	setOverflow(preAdd);

	tick();
	tick();
	tick();
	tick();
}

void Cpu::addc_abs_x(void)
{
	//TODO: check if page is crossed

}

void Cpu::addc_abs_y(void)
{
	//TODO: check if page is crossed
}

void Cpu::addc_ind_x(void)
{
	//TODO: check if page is crossed
}

void Cpu::addc_ind_y(void)
{
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
