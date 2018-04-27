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

//--------------------------------------------------------------------------
// Load/Store Operations
//--------------------------------------------------------------------------

/**
    * Performs the load accumulator instruction with the given addressing mode.
    *
    * @param mode: The addressing mode function, used to get the operand for
    * the store instruction.
    */
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
void inx_op(address_mode mode);
void iny_op(address_mode mode);
void dec_op(address_mode mode);
void dex_op(address_mode mode);
void dey_op(address_mode mode);
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
void jsr_op(address_mode mode);
void rts_op(address_mode mode);
//------------------------------------------------------------------------------
// Branch instructions
//------------------------------------------------------------------------------
void bcc_op(address_mode mode);
void bcs_op(address_mode mode);
void beq_op(address_mode mode);
void bmi_op(address_mode mode);
void bne_op(address_mode mode);
void bpl_op(address_mode mode);
void bvc_op(address_mode mode);
void bvs_op(address_mode mode);
//------------------------------------------------------------------------------
// Status flag change instructions
//------------------------------------------------------------------------------
void clc_op(address_mode mode);
void cld_op(address_mode mode);
void cli_op(address_mode mode);
void clv_op(address_mode mode);
void sec_op(address_mode mode);
void sed_op(address_mode mode);
void sei_op(address_mode mode);
//------------------------------------------------------------------------------
// System Functions
//------------------------------------------------------------------------------
void brk_op(address_mode mode);
/**
 * No operation.
 */
void nop_op(void);
void rti_op(address_mode mode);
	
	


} // end Cpu namespace

#endif
