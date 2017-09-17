#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>
#include <vector>

using namespace std;

enum ADDRESS_MODE {
	IMPLICIT,
	ACCUMULATOR,
	IMMEDIATE,
	ZERO_PAGE,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	RELATIVE,
	ABSOLUTE,
	ABSOLUTE_X,
	ABSOLUTE_Y,
	INDIRECT,
	INDEXED_INDIRECT,
	INDIRECT_INDEXED,
};

enum INSTRUCTIONS {
	ADC,	// add with carry
	AND,	// logical and 
	ASL,	// arithmetic shift left
	BCC,	// branch on carry clear
	BCS,	// branch on carry set
	BEQ,	// branch on equal (zero set)
	BIT,	// bit test
	BMI,	// branch on minus (negative set)
	BNE,	// branch on not equal (zero clear)
	BPL,	// branch on plus (negative clear)
	BRK,	// interrupt
	BVC,	// branch on overflow clear
	BVS,	// branch on overflow set
	CLC,	// clear carry
	CLD,	// clear decimal
	CLI,	// clear interrupt disable
	CLV,	// clear overflow
	CMP,	// compare 
	CPX,	// compare with X
	CPY,	// compary with Y
	DEC,	// decrement
	DEX,	// decrement X
	DEY,	// decrement Y
	EOR,	// exclusve or 
	INC,	// increment
	INX,	// increment X
	INY,	// increment Y
	JMP,	// jump
	JSR,	// jump subroutine
	LDA,	// load accumulator
	LDY,	// load Y
	LDX,	// load X
	LSR,	// logical shift right
	NOP,	// no operation
	ORA,	// logical or
	PHA,	// push accumulator
	PHP,	// push processor status
	PLA,	// pull accumulator
	PLP,	// pul processor status
	ROL,	// rotate left
	ROR,	// rotate right
	RTI,	// return from interrupt
	RTS,	// return from subroutine
	SBC,	// subtract with carry
	SEC,	// set carry
	SED,	// set decimal
	SEI,	// set interupt disable
	STA,	// store accumulator
	STX,	// store X
	STY,	// store Y
	TAX,	// transfer accumulator to X
	TAY,	// transfer accumulator to Y
	TSX,	// transfer stack pointer to X
	TXA,	// transfer X to accumulator
	TXS,	// transfer X to stack pointer
	TYA,	// transfer Y to accumulator
};

struct Opcode {
	uint8_t addressMode;
	uint8_t instruction;
	uint8_t cycles;

	Opcode(uint8_t am, uint8_t inst, uint8_t cyc) 
		: addressMode(am), instruction(inst), cycles(cyc) {};
	~Opcode();
};

vector<Opcode> DebugOpCodes;

/**
 * Opens the init file and initializes the vector of op-codes.
 * 		will be in the format:
 * 		inst address-mode cycles\n
 */
void initDebugOpcodes();

#endif













