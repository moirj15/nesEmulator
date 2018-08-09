#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include <vector>

namespace Cpu {

    enum FLAGS 
    {
	    CARRY_FLAG = 1,
	    ZERO_FLAG = 2,
	    INTERRUPT_DISSABLE_FLAG = 4,
	    DECIMAL_MODE_FLAG = 8,
        BREAK_FLAG = 32,
	    OVERFLOW_FLAG = 64,
	    NEGATIVE_FLAG = 128,
    };

    typedef u16 (*address_mode)(void);

    void init(void);
    void step(void);

    // getters for the cpu registers.
    u8 get_regA(void);
    u8 get_regX(void);
    u8 get_regY(void);
    u8 get_status(void);
    u8 get_sp(void);
    u16 get_pc(void);

    void testCpu(const std::vector<u8> &code);

    /**
     * Run the Cpu for a cycle.
     */
    void tick(void);

    // memory access methods
    // All memory access methods grab the address from the opCode's operands.
    // Once an opcode is read the memory methods will increment
    // the program counter and then use the new value in the program counter
    // to get the value that will be used for accessing memory.

    //TODO: write better method descriptions.

    u16 get_accumulator(void);
    /**
     * Grabs the immediate value from the next byte in program memory.
     *
     * @return: The value found at the next byte.
     */
    u16 get_immediate(void);

    /**
     * Grabs the next byte from program memory and uses that to retrieve a 
     * value from the zero page.
     *
     * @return: The value found in the zero page.
     */
    u16 get_zero_page(void);

    /**
     * Grabs the next byte from program memory and adds that to the value found
     * in the X register and uses that new value to retrieve a value from the
     * zero page.
     *
     * @return: The value found in the zero page.
     */
    u16 get_zero_page_x(void);

    u16 get_zero_page_y(void);

    /**
     * Grabs the 16 bit address from the next two bytes and uses that to access
     * a value in memory.
     *
     * @return: The value found at the absolute address.
     */
    u16 get_absolute(void);

    /**
     * Grabs the 16 bit address from the next two bytes and adds it to the 
     * value found in the X register and uses the calculated value to fetch a
     * value from memory.
     *
     * @return: The value found in memory.
     */
    u16 get_absolute_X_index(void);

    /**
     * Grabs the 16 bit address from the next two bytes and adds it to the 
     * value found in the y register and uses the calculated value to fetch a
     * value from memory.
     *
     * @return: The value found in memory.
     */
    u16 get_absolute_Y_index(void);

    u16 get_indirect(void);

    /**
     * The value in the X register is added to the next byte. This result is
     * used to grab a 16 bit value from the zero page which is then used to
     * access a value in memory.
     *
     * @return: The value found in memory.
     */
    u16 get_indexed_indirect(void);

    /**
     * Takes the value in the next byte, uses that to retrieve a 16 bit value
     * from the zero page and adds the value in the Y register to that. The 
     * result is then used to access a value in memory.
     *
     * @return: The value found in memory.
     */
    u16 get_indirect_indexed(void);

    // Flag operations

    void set_carry(u8 preAdd);
    void set_zero(u8 reg);
    void set_overflow(u8 reg);
    void set_negative(u8 reg);

    void new_page_cycle(u16 old_pc);
    void do_branch(s8 displacement, bool exp);

}	

#endif
