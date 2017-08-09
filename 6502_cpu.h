/**
 * FILE: 6502.h
 * AUTHOR: moirj15
 *
 * This header contains the definitions for the 6502's flags, opcodes, as well
 * as the forward declaration of the class's that handle the emulation of the 
 * cpu.
 */

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

#define LDA_IMM 0xA9	// Load Accumulator immediate address mode
#define LDA_ZP	0xA5	// Load Accumulator zero page address mode
#define LDA_ZPX	0xB5	// Load Accumulator zero page X address mode
#define LDA_IZX	0xA1	// Load Accumulator zero page X address mode
#define LDA_IZY	0xB1	// Load Accumulator zero page Y address mode
#define LDA_ABS	0xAD	// Load Accumulator absolute address mode
#define LDA_ABX	0xBD	// Load Accumulator absolute X address mode
#define LDA_ABY	0xB9	// Load Accumulator absolute Y address mode

#define LDX_IMM	0xA2	// Load X Register immediate address mode
#define LDX_ZP	0xA6	// Load X Register zero page address mode
#define LDX_ZPY	0xB6	// Load X Register zero page Y address mode
#define LDX_ABS	0xAE	// Load X Register absolute address mode
#define LDX_ABY	0xBE	// Load X Register absolute Y address mode

#define LDY_IMM	0xA0	// Load Y Register immediate address mode
#define LDY_ZP	0xA4	// Load Y Register zero page address mode
#define LDY_ZPY	0xB4	// Load Y Register zero page X address mode
#define LDY_ABS	0xAC	// Load Y Register absolute address mode
#define LDY_ABX	0xBC	// Load Y Register absolute X address mode

#define STA_ZP	0x85	// Store Accumulator zero page address mode
#define STA_ZPX	0x95	// Store Accumulator zero page X address mode
#define STA_IZX	0x81	// Store Accumulator zero page X address mode
#define STA_IZY	0x91	// Store Accumulator zero page Y address mode
#define STA_ABS	0x8D	// Store Accumulator absolute address mode
#define STA_ABX	0x9D	// Store Accumulator absolute X address mode
#define STA_ABY	0x99	// Store Accumulator absolute Y address mode

#define STX_ZP	0x86	// Store X Register zero page address mode
#define STX_ZPY	0x96	// Store X Register zero page Y address mode
#define STX_ABS	0x8E	// Store X Register absolute address mode

#define STY_ZP	0x84	// Store Y Register zero page address mode
#define STY_ZPX	0x94	// Store Y Register zero page X address mode
#define STY_ABS	0x8C	// Store Y Register absolute address mode

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

#define AND_IMM	0x29	// Logical AND immediate address mode
#define AND_ZP	0x25	// Logical AND zero page address mode
#define AND_ZPX	0x35	// Logical AND zero page X address mode
#define AND_IZX	0x21	// Logical AND indexed indirect address mode 
#define AND_IZY	0x31	// Logical AND indirect indexed address mode
#define AND_ABS	0x2D	// Logical AND absolute address mode
#define AND_ABX	0x3D	// Logical AND absolute X address mode
#define AND_ABY	0x39	// Logical AND absolute Y address mode

#define EOR_IMM	0x49	// Exclusive OR immediate address mode
#define EOR_ZP	0x45	// Exclusive OR zero page address mode
#define EOR_ZPX	0x55	// Exclusive OR zero page X address mode
#define EOR_IZX	0x41	// Exclusive OR indexed indirect address mode 
#define EOR_IZY	0x51	// Exclusive OR indirect indexed address mode
#define EOR_ABS	0x4D	// Exclusive OR absolute address mode
#define EOR_ABX	0x5D	// Exclusive OR absolute X address mode
#define EOR_ABY	0x59	// Exclusive OR absolute Y address mode

#define ORA_IMM	0x09	// Logical Inclusive OR immediate address mode
#define ORA_ZP	0x05	// Logical Inclusive OR zero page address mode
#define ORA_ZPX	0x15	// Logical Inclusive OR zero page X address mode
#define ORA_IZX	0x01	// Logical Inclusive OR indexed indirect address mode 
#define ORA_IZY	0x11	// Logical Inclusive OR indirect indexed address mode
#define ORA_ABS	0x0D	// Logical Inclusive OR absolute address mode
#define ORA_ABX	0x1D	// Logical Inclusive OR absolute X address mode
#define ORA_ABY	0x19	// Logical Inclusive OR absolute Y address mode

#define BIT_ZP	0x24	// Bit Test zero page address mode
#define BIT_ABS	0x2C	// Bit Test absolute address mode

// ARITHMETIC OPERATIONS

#define ADC_IMM	0x69	// Add with carry immediate address mode
#define ADC_ZP	0x65	// Add with carry zero page address mode
#define ADC_ZPX	0x75	// Add with carry zero page X address mode
#define ADC_IZX	0x61	// Add with carry indexed indirect address mode 
#define ADC_IZY	0x71	// Add with carry indirect indexed address mode
#define ADC_ABS	0x6D	// Add with carry absolute address mode
#define ADC_ABX	0x7D	// Add with carry absolute X address mode
#define ADC_ABY	0x79	// Add with carry absolute Y address mode

#define SBC_IMM	0xE9	// Subtract with carry immediate address mode
#define SBC_ZP	0xE5	// Subtract with carry zero page address mode
#define SBC_ZPX	0xF5	// Subtract with carry zero page X address mode
#define SBC_IZX	0xE1	// Subtract with carry indexed indirect address mode 
#define SBC_IZY	0xF1	// Subtract with carry indirect indexed address mode
#define SBC_ABS	0xED	// Subtract with carry absolute address mode
#define SBC_ABX	0xFD	// Subtract with carry absolute X address mode
#define SBC_ABY	0xF9	// Subtract with carry absolute Y address mode

#define CMP_IMM	0xC9	// Compare accumulator immediate address mode
#define CMP_ZP	0xC5	// Compare accumulator zero page address mode
#define CMP_ZPX	0xD5	// Compare accumulator zero page X address mode
#define CMP_IZX	0xC1	// Compare accumulator indexed indirect address mode 
#define CMP_IZY	0xD1	// Compare accumulator indirect indexed address mode
#define CMP_ABS	0xCD	// Compare accumulator absolute address mode
#define CMP_ABX	0xDD	// Compare accumulator absolute X address mode
#define CMP_ABY	0xD9	// Compare accumulator absolute Y address mode

#define CPX_IMM	0xE0	// Compare X register immediate address mode
#define CPX_ZP	0xE4	// Compare X register zero page address mode
#define CPX_ABS	0xEC	// Compare X register absolute address mode

#define CPY_IMM	0xC0	// Conpare Y register immediate address mode
#define CPY_ZP	0xC4	// Conpare Y register zero page address mode
#define CPY_ABS	0xCC	// Conpare Y register absolute address mode

// INCREMENT & DECREMENT OPERATIONS

#define INC_ZP	0xE6	// Increment a memory location zero page address mode
#define INC_ZPX	0xF6	// Increment a memory location zero page X address mode
#define INC_ABS	0xEE	// Increment a memory location absolute address mode
#define INC_ABX	0xFE	// Increment a memory location absolute X address mode

#define INX		0xE8	// Increment the X register
#define INY		0xC8	// Increment the Y register

#define DEC_ZP	0xC6	// Decrement a memory location zero page address mode
#define DEC_ZPX	0xD6	// Decrement a memory location zero page X address mode
#define DEC_ABS	0xCE	// Decrement a memory location absolute address mode
#define DEC_ABX	0xDE	// Decrement a memory location absolute X address mode

#define DEX		0xCA	// Decrement the X register
#define DEY		0x88	// Decrement the Y register

// SHIFT OPERATIONS

#define ASL_IMP	0x0A	// Arithmetic shift left implicit address mode
#define ASL_ZP	0x06	// Arithmetic shift left zero page address mode
#define ASL_ZPX	0x16	// Arithmetic shift left zero page X address mode
#define ASL_ABS	0x0E	// Arithmetic shift left absolute address mode
#define ASL_ABX	0x1E	// Arithmetic shift left absolute X address mode

#define LSR_IMP	0x4A	// Logical shift right implicit address mode
#define LSR_ZP	0x46	// Logical shift right zero page address mode
#define LSR_ZPX	0x56	// Logical shift right zero page X address mode
#define LSR_ABS	0x4E	// Logical shift right absolute address mode
#define LSR_ABX	0x5E	// Logical shift right absolute X address mode

#define ROL_IMP	0x2A	// Rotate left implicit address mode
#define ROL_ZP	0x26	// Rotate left zero page address mode
#define ROL_ZPX	0x36	// Rotate left zero page X address mode
#define ROL_ABS	0x2E	// Rotate left absolute address mode
#define ROL_ABX	0x3E	// Rotate left absolute X address mode

#define ROR_IMP	0x6A	// Rotate right implicit address mode
#define ROR_ZP	0x66	// Rotate right zero page address mode
#define ROR_ZPX	0x76	// Rotate right zero page X address mode
#define ROR_ABS	0x6E	// Rotate right absolute address mode
#define ROR_ABX	0x7E	// Rotate right absolute X address mode

// JUMPS AND CALLS

#define JMP_ABS	0x4C	// Jumpt to another location absolute address mode
#define JMP_IND	0x6C	// Jumpt to another location indirect address mode

#define JSR		0x20	// Jump to a subroutine
#define RTS		0x60	// Return from subroutine

// BRANCH OPERATIONS

#define BCC		0x90	// Branch if carry flag clear
#define BCS		0xB0	// Branch if carry flag set
#define BEQ		0xF0	// Branch if zero flag set
#define BMI		0x30	// Branch if negative flag set
#define BNE		0xD0	// Branch if zero flag clear
#define BPL		0x10	// Branch if negative flag clear
#define BVC		0x50	// Branch if overflow flag clear
#define BVS		0x70	// Branch if overflow flag set

// STATUS FLAG CHANGES

#define CLC		0x18	// Clear carry flag
#define CLD		0xD8	// Clear decimal mode flag
#define CLI		0x58	// Clear interupt disable flag
#define CLV		0xB8	// Clear overflow flag
#define SEC		0x38	// Set carry flag
#define SED		0xF8	// Set decimal mode flag
#define SEI		0x78	// Set interrupt disable flag

// SYSTEM FUNCTIONS

#define BRK		0x00	// Force an interrupt
#define NOP		0xEA	// No Operation
#define RTI		0x40	// Return from interrupt

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

	Registers(void);
};

class Cpu {

public:
	Cpu();
	~Cpu();

private:
	Registers *reg;
};

#endif
