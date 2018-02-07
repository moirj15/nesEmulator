#ifndef CPU_H
#define CPU_H

#include "utils.h"

enum FLAGS 
{
	CARRY = 1,
	ZERO = 2,
	INTERRUPT_DISSABLE = 4,
	DECIMAL_MODE = 8,
	OVERFLOW = 64,
	NEGATIVE = 128,
};

class Cpu
{
	// General purpose registers
	u8 	A;
	u8 	X;
	u8 	Y;

	// Special purpose registers
	u8 	sp;
	u8 	status;
	u16 pc;

	u8 memory[0x10000]; // for now allocate the entire address space for the emulator

	const s32 CYCLES_PER_FRAME = 29834;
	s32 remainingCycles; //borrowed from github/AndreaOrru/LaiNES 

public:
	Cpu(void);
	~Cpu(void);

	void run(void);
	
private:
	/**
 	 * Run the Cpu for a cycle.
 	 */
	void tick(void);

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
	u8 getImmediate(void);

	/**
	 * Grabs the next byte from program memory and uses that to retrieve a 
	 * value from the zero page.
	 *
	 * @return: The value found in the zero page.
	 */
	u8 getZeroPage(void);

	/**
	 * Grabs the next byte from program memory and adds that to the value found
	 * in the X register and uses that new value to retrieve a value from the
	 * zero page.
	 *
	 * @return: The value found in the zero page.
	 */
	u8 getZeroPageIndexed(void);

	/**
	 * Grabs the 16 bit address from the next two bytes and uses that to access
	 * a value in memory.
	 *
	 * @return: The value found at the absolute address.
	 */
	u8 getAbsolute(void);

	/**
	 * Grabs the 16 bit address from the next two bytes and adds it to the 
	 * value found in the X register and uses the calculated value to fetch a
	 * value from memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 getAbsoluteXIndex(void);

	/**
	 * Grabs the 16 bit address from the next two bytes and adds it to the 
	 * value found in the y register and uses the calculated value to fetch a
	 * value from memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 getAbsoluteYIndex(void);

	/**
	 * The value in the X register is added to the next byte. This result is
	 * used to grab a 16 bit value from the zero page which is then used to
	 * access a value in memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 getIndexedIndirect(void);

	/**
	 * Takes the value in the next byte, uses that to retrieve a 16 bit value
	 * from the zero page and adds the value in the Y register to that. The 
	 * result is then used to access a value in memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 getIndirectIndexed(void);

	/**
	 * Add with carry instructions.
	 */
	void addc_imm(void);
	void addc_zp(void);
	void addc_zpx(void);
	void addc_abs(void);
	void addc_abs_x(void);
	void addc_abs_y(void);
	void addc_ind_x(void);
	void addc_ind_y(void);

	/**
	 * No operation.
	 */
	void nop(void);

};


#endif
