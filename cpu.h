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
    void run(void);

    void testCpu(const std::vector<u8> &code);

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


    //--------------------------------------------------------------------------
    // Load/Store Operations
    //--------------------------------------------------------------------------

    void lda_op(address_mode mode);
    void ldx_op(address_mode mode);
    void ldy_op(address_mode mode);
    void sta_op(address_mode mode);
    void stx_op(address_mode mode);
    void sty_op(address_mode mode);


    //--------------------------------------------------------------------------
    // Register Transfer instructions
    //--------------------------------------------------------------------------
    void tax_op(void);
    void tay_op(void);
    void txa_op(void);
    void tya_op(void);


    //--------------------------------------------------------------------------
    // Stack operations
    //--------------------------------------------------------------------------

    void tsx_op(void);
    void txs_op(void);
    void pha_op(void);
    void php_op(void);
    void pla_op(void);
    void plp_op(void);
        
        
    //------------------------------------------------------------------------------
    // Logical operations
    //------------------------------------------------------------------------------
    void and_op(address_mode mode);
    void eor_op(address_mode mode);
    void ora_op(address_mode mode);
    void bit_op(address_mode mode);

    //------------------------------------------------------------------------------
    // Arithmetic instructions
    //------------------------------------------------------------------------------
    /**
     * Performs the add with carry instruction with the given addressing mode.
     *
     * @param mode: The addressing mode function, used to get the operand for
     * the addition.
     */
    void adc_op(address_mode mode);
    void sbc_op(address_mode mode);
    //------------------------------------------------------------------------------
    // Compare instructions
    //------------------------------------------------------------------------------
    void cmp_op(address_mode mode);
    void cpx_op(address_mode mode);
    void cpy_op(address_mode mode);
    //------------------------------------------------------------------------------
    // Increment and Decrement instructions
    //------------------------------------------------------------------------------
    void inc_op(address_mode mode);
    void inx_op();
    void iny_op();
    void dec_op(address_mode mode);
    void dex_op();
    void dey_op();
    //------------------------------------------------------------------------------
    // Shift instructions
    //------------------------------------------------------------------------------
    void asl_op(address_mode mode);
    void lsr_op(address_mode mode);
    void rol_op(address_mode mode);
    void ror_op(address_mode mode);
    //------------------------------------------------------------------------------
    // Jump and call instructions
    //------------------------------------------------------------------------------
    void jmp_op(address_mode mode);
    void jsr_op(void);
    void rts_op(void);
    //------------------------------------------------------------------------------
    // Branch instructions
    //------------------------------------------------------------------------------
    void bcc_op(void);
    void bcs_op(void);
    void beq_op(void);
    void bmi_op(void);
    void bne_op(void);
    void bpl_op(void);
    void bvc_op(void);
    void bvs_op(void);
    //------------------------------------------------------------------------------
    // Status flag change instructions
    //------------------------------------------------------------------------------
    void clc_op(void);
    void cld_op(void);
    void cli_op(void);
    void clv_op(void);
    void sec_op(void);
    void sed_op(void);
    void sei_op(void);
    //------------------------------------------------------------------------------
    // System Functions
    //------------------------------------------------------------------------------
    void brk_op(void);
    void nop_op(void);
    void rti_op(void);
	
}	

#endif
