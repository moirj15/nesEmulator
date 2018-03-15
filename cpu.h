#ifndef CPU_H
#define CPU_H

#include "utils.h"

namespace Cpu {

enum FLAGS 
{
	CARRY = 1,
	ZERO = 2,
	INTERRUPT_DISSABLE = 4,
	DECIMAL_MODE = 8,
	OVERFLOW = 64,
	NEGATIVE = 128,
};

typedef u8 (*address_mode)(void);

	void init(void);
	void run(void);

	void testCpu(u8 *code, u32 size);
	
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
	u8 get_immediate(void);

	/**
	 * Grabs the next byte from program memory and uses that to retrieve a 
	 * value from the zero page.
	 *
	 * @return: The value found in the zero page.
	 */
	u8 get_zero_page(void);

	/**
	 * Grabs the next byte from program memory and adds that to the value found
	 * in the X register and uses that new value to retrieve a value from the
	 * zero page.
	 *
	 * @return: The value found in the zero page.
	 */
	u8 get_zero_page_indexed(void);

	/**
	 * Grabs the 16 bit address from the next two bytes and uses that to access
	 * a value in memory.
	 *
	 * @return: The value found at the absolute address.
	 */
	u8 get_absolute(void);

	/**
	 * Grabs the 16 bit address from the next two bytes and adds it to the 
	 * value found in the X register and uses the calculated value to fetch a
	 * value from memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 get_absolute_X_index(void);

	/**
	 * Grabs the 16 bit address from the next two bytes and adds it to the 
	 * value found in the y register and uses the calculated value to fetch a
	 * value from memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 get_absolute_Y_index(void);

	/**
	 * The value in the X register is added to the next byte. This result is
	 * used to grab a 16 bit value from the zero page which is then used to
	 * access a value in memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 get_indexed_indirect(void);

	/**
	 * Takes the value in the next byte, uses that to retrieve a 16 bit value
	 * from the zero page and adds the value in the Y register to that. The 
	 * result is then used to access a value in memory.
	 *
	 * @return: The value found in memory.
	 */
	u8 get_indirect_indexed(void);

	// Flag operations

	void set_carry(u8 preAdd);
	void set_zero(u8 reg);
	void set_overflow(u8 reg);
	void set_negative(u8 reg);

	// load/store operations

	/**
	 * Performs the load accumulator instruction with the given addressing mode.
	 *
	 * @param mode: The addressing mode function, used to get the operand for
	 * the store instruction.
	 */
	void lda(address_mode mode);
	void ldx(address_mode mode);
	void ldy(address_mode mode);
	void sta(address_mode mode);
	void stx(address_mode mode);
	void sty(address_mode mode);

	// Register Transfer instructions
	
	void tax(address_mode mode);
	void tay(address_mode mode);
	void txa(address_mode mode);
	void tya(address_mode mode);


	/**
	 * Performs the add with carry instruction with the given addressing mode.
	 *
	 * @param mode: The addressing mode function, used to get the operand for
	 * the addition.
	 */
	void addc(address_mode mode);
	
	

	/**
	 * No operation.
	 */
	void nop(void);

} // end Cpu namespace

#endif
