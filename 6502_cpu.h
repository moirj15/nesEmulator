#ifndef SFOT_CPU
#define SFOT_CPU

#define CARRY_FLAG		1	// for when the last operation results in a carry
#define ZERO_FLAG		2	// for when the last operation results in a zero
#define INTERUPT_FLAG	4	// when set interupts are dissabled
#define DECIMAL_FLAG	8	// changes form of arithmetic
#define BREAK_FLAG		16	// occurs when a BRK instruction is executed
#define NEGATIVE_FLAG	32	// occurs when bit 7 of the last op is 1

///////////////////////////////////////////////////////////////////////////////
//								6502 opcodes								 //
///////////////////////////////////////////////////////////////////////////////

// LOAD/STORE OPERATIONS

#define LDA_IMM 0x	// Load Accumulator immediate address mode
#define LDA_ZP	0x	// Load Accumulator zero page address mode
#define LDA_ZPX	0x	// Load Accumulator zero page X address mode
#define LDA_IZY	0x	// Load Accumulator zero page Y address mode
#define LDA_ABS	0x	// Load Accumulator absolute address mode
#define LDA_ABX	0x	// Load Accumulator absolute X address mode
#define LDA_ABY	0x	// Load Accumulator absolute Y address mode

#define LDX_IMM	0x	// Load X Register immediate address mode
#define LDX_ZP	0x	// Load X Register zero page address mode
#define LDX_ZPY	0x	// Load X Register zero page Y address mode
#define LDX_ABS	0x	// Load X Register absolute address mode
#define LDX_ABY	0x	// Load X Register absolute Y address mode

#define LDY_IMM	0x	// Load Y Register immediate address mode
#define LDY_ZP	0x	// Load Y Register zero page address mode
#define LDY_ZPY	0x	// Load Y Register zero page X address mode
#define LDY_ABS	0x	// Load Y Register absolute address mode
#define LDY_ABX	0x	// Load Y Register absolute X address mode

#define STA_ZP	0x	// Store Accumulator zero page address mode
#define STA_ZPX	0x	// Store Accumulator zero page X address mode
#define STA_IZY	0x	// Store Accumulator zero page Y address mode
#define STA_ABS	0x	// Store Accumulator absolute address mode
#define STA_ABX	0x	// Store Accumulator absolute X address mode
#define STA_ABY	0x	// Store Accumulator absolute Y address mode

#define STX_ZP	0x	// Store X Register zero page address mode
#define STX_ZPY	0x	// Store X Register zero page Y address mode
#define STX_ABS	0x	// Store X Register absolute address mode

#define STY_ZP	0x	// Store Y Register zero page address mode
#define STY_ZPX	0x	// Store Y Register zero page X address mode
#define STY_ABS	0x	// Store Y Register absolute address mode

// REGISTER TRANSFERS

#define TAX		0xAA	// Transfer accumulator to X
#define TAY		0xA8	// Transfer accumulator to Y
#define TXA		0x8A	// Transfer X to accumulator
#define TYA		0x98	// Transfer Y to accumulator

// STACK OPERATIONS

#define TSX		0xBA	// Transfer stack pointer to X
#define TXS		0x9A	// Transfer X to stack pointer
#define PHA		0x48	// Push accumulator on stack
#define PHP		0x08	// Push processor status on stack 
#define PLA		0x68	// Pull accumulator from stack
#define PLP		0x28	// Pull processor status from stack

// LOGICAL OPERATIONS

#define AND_IMM	0x	// Logical AND immediate address mode
#define AND_ZP	0x	// Logical AND zero page address mode
#define AND_ZPX	0x	// Logical AND zero page X address mode
#define AND_IZX	0x	// Logical AND indexed indirect address mode 
#define AND_IZY	0x	// Logical AND indirect indexed address mode
#define AND_ABS	0x	// Logical AND absolute address mode
#define AND_ABX	0x	// Logical AND absolute X address mode
#define AND_ABY	0x	// Logical AND absolute Y address mode

#define EOR_IMM	0x	// Exclusive OR immediate address mode
#define EOR_ZP	0x	// Exclusive OR zero page address mode
#define EOR_ZPX	0x	// Exclusive OR zero page X address mode
#define EOR_IZX	0x	// Exclusive OR indexed indirect address mode 
#define EOR_IZY	0x	// Exclusive OR indirect indexed address mode
#define EOR_ABS	0x	// Exclusive OR absolute address mode
#define EOR_ABX	0x	// Exclusive OR absolute X address mode
#define EOR_ABY	0x	// Exclusive OR absolute Y address mode

#define ORA_IMM	0x	// Logical Inclusive OR immediate address mode
#define ORA_ZP	0x	// Logical Inclusive OR zero page address mode
#define ORA_ZPX	0x	// Logical Inclusive OR zero page X address mode
#define ORA_IZX	0x	// Logical Inclusive OR indexed indirect address mode 
#define ORA_IZY	0x	// Logical Inclusive OR indirect indexed address mode
#define ORA_ABS	0x	// Logical Inclusive OR absolute address mode
#define ORA_ABX	0x	// Logical Inclusive OR absolute X address mode
#define ORA_ABY	0x	// Logical Inclusive OR absolute Y address mode

#define BIT_ZP	0x	// Bit Test zero page address mode
#define BIT_ABS	0x	// Bit Test absolute address mode

// ARITHMETIC OPERATIONS

#define ADC_IMM	0x	// Add with carry immediate address mode
#define ADC_ZP	0x	// Add with carry zero page address mode
#define ADC_ZPX	0x	// Add with carry zero page X address mode
#define ADC_IZX	0x	// Add with carry indexed indirect address mode 
#define ADC_IZY	0x	// Add with carry indirect indexed address mode
#define ADC_ABS	0x	// Add with carry absolute address mode
#define ADC_ABX	0x	// Add with carry absolute X address mode
#define ADC_ABY	0x	// Add with carry absolute Y address mode

#define SBC_IMM	0x	// Subtract with carry immediate address mode
#define SBC_ZP	0x	// Subtract with carry zero page address mode
#define SBC_ZPX	0x	// Subtract with carry zero page X address mode
#define SBC_IZX	0x	// Subtract with carry indexed indirect address mode 
#define SBC_IZY	0x	// Subtract with carry indirect indexed address mode
#define SBC_ABS	0x	// Subtract with carry absolute address mode
#define SBC_ABX	0x	// Subtract with carry absolute X address mode
#define SBC_ABY	0x	// Subtract with carry absolute Y address mode

#define CMP_IMM	0x	// Compare accumulator immediate address mode
#define CMP_ZP	0x	// Compare accumulator zero page address mode
#define CMP_ZPX	0x	// Compare accumulator zero page X address mode
#define CMP_IZX	0x	// Compare accumulator indexed indirect address mode 
#define CMP_IZY	0x	// Compare accumulator indirect indexed address mode
#define CMP_ABS	0x	// Compare accumulator absolute address mode
#define CMP_ABX	0x	// Compare accumulator absolute X address mode
#define CMP_ABY	0x	// Compare accumulator absolute Y address mode

#define CPX_IMM	0x	// Compare X register immediate address mode
#define CPX_ZP	0x	// Compare X register zero page address mode
#define CPX_ABS	0x	// Compare X register absolute address mode

#define CPY_IMM	0x	// Conpare Y register immediate address mode
#define CPY_ZP	0x	// Conpare Y register zero page address mode
#define CPY_ABS	0x	// Conpare Y register absolute address mode

// INCREMENT & DECREMENT OPERATIONS

#define INC_ZP		0x	// Increment a memory location zero page address mode
#define INC_ZPX		0x	// Increment a memory location zero page X address mode
#define INC_ABS		0x	// Increment a memory location absolute address mode
#define INC_ABX		0x	// Increment a memory location absolute X address mode

#define INX			0x	// Increment the X register
#define INY			0x	// Increment the Y register

#define DEC_ZP		0x	// Decrement a memory location zero page address mode
#define DEC_ZPX		0x	// Decrement a memory location zero page X address mode
#define DEC_ABS		0x	// Decrement a memory location absolute address mode
#define DEC_ABX		0x	// Decrement a memory location absolute X address mode

#define DEX			0x	// Decrement the X register
#define DEY			0x	// Decrement the Y register

// SHIFT OPERATIONS

#define ASL_IMP		0x	// Arithmetic shift left implicit address mode
#define ASL_ZP		0x	// Arithmetic shift left zero page address mode
#define ASL_ZPX		0x	// Arithmetic shift left zero page X address mode
#define ASL_ABS		0x	// Arithmetic shift left absolute address mode
#define ASL_ABX		0x	// Arithmetic shift left absolute X address mode

#define LSR_IMP		0x	// Logical shift right implicit address mode
#define LSR_ZP		0x	// Logical shift right zero page address mode
#define LSR_ZPX		0x	// Logical shift right zero page X address mode
#define LSR_ABS		0x	// Logical shift right absolute address mode
#define LSR_ABX		0x	// Logical shift right absolute X address mode

#define ROL_IMP		0x	// Rotate left implicit address mode
#define ROL_ZP		0x	// Rotate left zero page address mode
#define ROL_ZPX		0x	// Rotate left zero page X address mode
#define ROL_ABS		0x	// Rotate left absolute address mode
#define ROL_ABX		0x	// Rotate left absolute X address mode

#define ROR_IMP		0x	// Rotate right implicit address mode
#define ROR_ZP		0x	// Rotate right zero page address mode
#define ROR_ZPX		0x	// Rotate right zero page X address mode
#define ROR_ABS		0x	// Rotate right absolute address mode
#define ROR_ABX		0x	// Rotate right absolute X address mode

// JUMPS AND CALLS

#define JMP_ABS		0x	// Jumpt to another location absolute address mode
#define JMP_IND		0x	// Jumpt to another location indirect address mode

#define JSR			0x	// Jump to a subroutine
#define RTS			0x	// Return from subroutine

// BRANCH OPERATIONS

#define BCC			0x	// Branch if carry flag clear
#define BCS			0x	// Branch if carry flag set
#define BEQ			0x	// Branch if zero flag set
#define BMI			0x	// Branch if negative flag set
#define BNE			0x	// Branch if zero flag clear
#define BPL			0x	// Branch if negative flag clear
#define BVC			0x	// Branch if overflow flag clear
#define BVS			0x	// Branch if overflow flag set

// STATUS FLAG CHANGES

#define CLC			0x	// Clear carry flag
#define CLD			0x	// Clear decimal mode flag
#define CLI			0x	// Clear interupt disable flag
#define CLV			0x	// Clear overflow flag
#define SEC			0x	// Set carry flag
#define SED			0x	// Set decimal mode flag
#define SEI			0x	// Set interrupt disable flag

// SYSTEM FUNCTIONS

#define BRK			0x	// Force an interrupt
#define NOP			0x	// No Operation
#define RTI			0x	// Return from interrupt

/**
 * Struct containing the registers for the 6502 processor.
 */
struct Registers {
	unsigned short pc;
	unsigned char sp;
	char accumulator;
	char x;
	char y;
	unsigned char flags;
};



#endif
